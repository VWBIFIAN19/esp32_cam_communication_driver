#include <cinttypes>
#include <stddef.h>
#include "definitions.h"

#ifndef PACKETS_H
#define PACKETS_H

#define EMPTY_PACKET                (struct Packets::Packet) { 0, nullptr, 0 }
#define EMTPY_QUEUE                 (struct Packets::PacketQueueItem) { nullptr, nullptr }

#define TRANSMISSION_TIME(bytes)    (uint32_t) ((double) 1000 / (double) SERIAL_BPS * bytes)    //Time it should take to receive a given amount of bytes
#define TIMEOUT                     3000                                                        //The time in which all packets have to be received (+ TRANSMISSION_TIME)

namespace Packets {
    /**
     * Can be used to either describe a packet that is going to be sent or received
     */
    struct Packet {
        uint32_t packetId;      //Used to identify the type of packet
        uint8_t* buffer;        //Contains the packet's data
        uint32_t bufferSize;    //Amount of bytes that are contained by the buffer
    };

    /**
     * Is used to organize packets before you send them
     */
    struct PacketQueueItem {
        Packet* packet;                 //The Packet that is saved in the queue
        PacketQueueItem* next;          //Reference to the next item in the queue
    };

    class PacketHandler {
        public:
            PacketHandler(Packet* packet);
            Packet* packet;         //Received packet

            /**
             * Handles received packets
             * 
             * @return true if the handling was successful -> false if not
             */
            virtual bool Handle() = 0;
    };

    /*Packet handling*/

    /**
     * Create a handler for the given packet
     * 
     * @param packet the packet that is used in order to create the handler
     * @return pointer to the handler
     */
    void* CreatePacketHandler(Packet* packet);

    /**
     * Handles the incoming verified packets
     * 
     * @param buffer contains the sent data
     * @param size the size of the given buffer
     * @return true if the handling was successful -> false if not
     */
    bool HandlePacket(uint8_t* buffer, uint32_t size);

    /**
     * Frequently checks the serial input and decides if the received data is useful or not
     * also handles the outgoing packets
     */
    void HandleTransmission();

    /*Packet Sending*/

    /**
     * Creates a packet
     * 
     * @param buffer the outgoing bytes
     * @param size the number of outgoing bytes
     * @return the final packet
     */
    Packet* CreatePacket(uint32_t packetId, void* buffer, uint32_t size);

    /**
     * Creates a message packet
     * 
     * @param message the message
     * @return the final packet
     */
    Packet* CreateMessagePacket(char* message);

    /**
     * Creates an error packet
     * 
     * @param error the error message
     * @return the final packet
     */
    Packet* CreateErrorPacket(char* error);

    /**
     * Creates and sends an info packet of the given packet
     * 
     * @param packet the packet that is going to be sent
     */
    void SendInfoPacket(Packet* packet);

    /**
     * Prepares and sends the given packet
     * 
     * @param packet the packet that is going to be sent
     */
    void SendPacket(Packet* packet);

    /**
     * Add's a packet to the send queue
     * 
     * @param packet the desired packet
     */
    void AddPacketToSendQueue(Packet* packet);

    /**
     * Decides when to send the queued packets
     */
    void HandleSendQueue();
}

#endif