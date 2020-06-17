#include <cinttypes>
#include "Arduino.h"
#include "esp_camera.h"

#include "../../headers/packets.h"
#include "../../headers/packet_definitions.h"
#include "../../headers/handlers/handler001requestpicture.h"

/*Variables*/
bool frequentPictureStream = false;

Packets::Handler001RequestPicture::Handler001RequestPicture(Packet* packet) : PacketHandler(packet) {}

bool Packets::Handler001RequestPicture::Handle() {
    packet001_request_picture_t* request = (packet001_request_picture_t*) malloc(this->packet->bufferSize);
    memcpy((void*) request, (void*) this->packet->buffer, sizeof(packet001_request_picture_t));

    switch (request->request)
    {
        case packet_001_requests::REQUEST_ONE_PICTURE:
        {
            camera_fb_t* fb = esp_camera_fb_get();

            if(!fb) {
                Packets::AddPacketToSendQueue(Packets::CreateErrorPacket("001.0: Could not take picture"));
                return false;
            }

            Packets::AddPacketToSendQueue(Packets::CreatePacket(1, (void*) fb->buf, fb->len));

            esp_camera_fb_return(fb);
            return true;
        }
        case packet_001_requests::REQUEST_START_STREAM:
            frequentPictureStream = true;
            return true;
        case packet_001_requests::REQUEST_STOP_STREAM:
            frequentPictureStream = false;
            return true;
    }

    char* error;
    asprintf(&error, "001.1: Unknown request, request=%i", request->request);
    Packets::AddPacketToSendQueue(Packets::CreateErrorPacket(error));

    return false;
}

void Packets::HandlerRequestPicture::StreamPicture() {
    if(!frequentPictureStream) {
        return;
    }

    camera_fb_t* fb = esp_camera_fb_get();

    if(!fb) {
        Packets::AddPacketToSendQueue(Packets::CreateErrorPacket("001.0: Could not take picture"));
        return;
    }

    Packets::AddPacketToSendQueue(Packets::CreatePacket(1, (void*) fb->buf, fb->len));
    esp_camera_fb_return(fb);
}