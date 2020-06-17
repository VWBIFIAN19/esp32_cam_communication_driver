#include <cinttypes>
#include "Arduino.h"
#include "esp_camera.h"

#include "../../headers/packets.h"
#include "../../headers/handlers/handler000setupcamera.h"

/*Variables*/
bool setupComplete = false;
Packets::Handler000SetupCamera::Handler000SetupCamera(Packet* packet) : PacketHandler(packet) {}

//TODO: Force Reset Packet

bool Packets::Handler000SetupCamera::Handle() {
    if(setupComplete) {
        Packets::AddPacketToSendQueue(Packets::CreateErrorPacket("000.1: Cannot setup camera more than once"));
        return;
    }

    camera_config_t* config = (camera_config_t*) malloc(sizeof(camera_config_t));
    memcpy((void*) config, (void*) this->packet->buffer, sizeof(camera_config_t));

    esp_err_t err = esp_camera_init(config);
  
    if (err != ESP_OK) {
        Packets::AddPacketToSendQueue(Packets::CreateErrorPacket("000.0: Could not setup camera"));
        return false;
    }

    Packets::AddPacketToSendQueue(Packets::CreateMessagePacket("000.0: Success"));
    setupComplete = true;
    return true;
}