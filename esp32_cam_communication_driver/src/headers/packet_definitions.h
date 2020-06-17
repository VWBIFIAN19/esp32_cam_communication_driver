#ifndef PACKET_DEFINITIONS_H
#define PACKET_DEFINITIONS_H

namespace Packets {

    /*INFO Packet*/
    typedef struct {
        int32_t broadcastId;
        uint32_t packetId;
        uint32_t packetSize;
    } info_packet_t;

    /*PACKET 001 REQUEST PICTURE*/
    enum class packet_001_requests {
        REQUEST_ONE_PICTURE,
        REQUEST_START_STREAM,
        REQUEST_STOP_STREAM
    };

    typedef struct {
        packet_001_requests request;
    } packet001_request_picture_t;
}

#endif