#include "../packets.h"

#ifndef HANDLER000SETUPCAMERA_H
#define HANDLER000SETUPCAMERA_H

namespace Packets {
    class Handler000SetupCamera : public PacketHandler {
        public:
            Handler000SetupCamera(Packet* packet);
            bool Handle();
    };
}

#endif