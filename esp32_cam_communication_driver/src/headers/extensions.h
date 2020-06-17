#include "packets.h"

#ifndef EXTENSIONS_H
#define EXTENSIONS_H

namespace Extensions {
    /**
     * Once called at the start of the driver
     */
    void Setup();

    /**
     * Updates frequently at around 117 kHz -> 117.000 times per second
     */
    void Update();

    /**
     * Create a handler for the given packet
     * 
     * @param packet the packet that is used in order to create the handler
     * @return pointer to the handler
     */
    void* CreatePacketHandler(Packets::Packet* packet);
}

#endif