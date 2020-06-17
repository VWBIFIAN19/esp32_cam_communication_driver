#include <cinttypes>
#include "../headers/packets.h"

/*Handler*/
#include "../headers/handlers/handler000setupcamera.h"
#include "../headers/handlers/handler001requestpicture.h"
#include "../headers/extensions.h"

void* Packets::CreatePacketHandler(Packet* packet) {
    switch (packet->packetId)
    {
        case 0: return new Packets::Handler000SetupCamera(packet);
        case 1: return new Packets::Handler001RequestPicture(packet);
    }

    return Extensions::CreatePacketHandler(packet); //Check for extensions
}