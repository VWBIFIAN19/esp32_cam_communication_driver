#include "../packets.h"

#ifndef HANDLER001REQUESTPICTURE_H
#define HANDLER001REQUESTPICTURE_H

namespace Packets {
    class Handler001RequestPicture : public PacketHandler {
        public:
            Handler001RequestPicture(Packet* packet);
            bool Handle();
    };

    namespace HandlerRequestPicture {
        void StreamPicture();
    }
}

#endif