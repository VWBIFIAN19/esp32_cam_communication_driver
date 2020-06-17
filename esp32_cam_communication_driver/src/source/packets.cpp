#include <cinttypes>
#include "Arduino.h"

#include "../headers/packets.h"
#include "../headers/handlers.h"
#include "../headers/definitions.h"
#include "../headers/utils.h"
#include "../headers/packet_definitions.h"

/*Variables*/
bool isAwaitingPacket = false;                      //indicates if the driver is awaiting a packet, triggered by info packet
Packets::Packet awaitedPacket = EMPTY_PACKET;
uint32_t bytesLeft = 0;                             //the number of bytes the handler is still waiting for (only used if isAwaitingPacket equals true)

Packets::PacketQueueItem* packetQueue = nullptr;    //Describes the order in which packets are going to be sent

//Packets::PacketHandler updatedHandlers =

/*Classes*/
Packets::PacketHandler::PacketHandler(Packet* packet) {
    this->packet = packet;
};

/*Packet Handling*/
bool Packets::HandlePacket(uint8_t* buffer, uint32_t size) {
    /*Check if packet is valid*/
    if(!isAwaitingPacket && size == sizeof(info_packet_t) - sizeof(int32_t)) {
        info_packet_t* info_packet = (info_packet_t*) malloc(sizeof(info_packet_t));
        memcpy((void*) ((void*) info_packet + sizeof(int32_t)), buffer, sizeof(info_packet_t) - sizeof(int32_t)); //arg0 -> Add size of int32, because we do not have the broadcast Id in the buffer anymore

        /*Check if packet isn't empty*/
        if(info_packet->packetSize > 0 && info_packet->packetSize <= MAX_PACKET_SIZE) {
            isAwaitingPacket = true;
            bytesLeft = info_packet->packetSize;
            awaitedPacket = { info_packet->packetId, (uint8_t*) malloc(info_packet->packetSize), info_packet->packetSize };

            free(info_packet);
            return true;
        }

        free(info_packet);
    }else if(isAwaitingPacket) {
        memcpy((void*) (awaitedPacket.buffer + awaitedPacket.bufferSize - bytesLeft), buffer, size); //Add all bytes out of the packet into the awaited Packet
        bytesLeft -= size;

        free(buffer); //Free buffer as fast as possible to avoid too much memory usage

        if(bytesLeft == 0) {
            /*Handling*/
            Packets::Packet* pPacket = (Packets::Packet*) malloc(sizeof(Packets::Packet));     //Create a copy of the received packet
            memcpy((void*) pPacket, (void*) &awaitedPacket, sizeof(Packets::Packet));

            Packets::PacketHandler* pHandler = (Packets::PacketHandler*) Packets::CreatePacketHandler(pPacket);

            if(pHandler) {
                pHandler->Handle();
                free(pHandler);
            }

            /*Reset*/
            isAwaitingPacket = false;

            free(awaitedPacket.buffer);
            free(pPacket);

            awaitedPacket = EMPTY_PACKET;
            return true;
        }
    }else {
        free(buffer);
    }

    return false;
}


void Packets::HandleTransmission() {
    static uint8_t* broadcastIdBuffer = Utils::GetBufferFromInt32(BROADCAST_ID); //Used to identify info packets as early as possible
    static int32_t receivedLen = 0;     //Used to store the amount of bytes that matched the broadcastId so far
    static uint32_t timeout = 0;        //Bytes received after this time will not be added to the last packet

    uint32_t currentMillis = millis();

    int32_t available = Serial.available();

    if(!isAwaitingPacket && available > 0 && receivedLen < 4) {
        /*Check bytes, if it isn't waiting for an info packet*/
        int _available = available;

        for(int i = 0; i < _available; i++) {
            uint8_t read = Serial.read();

            if(!receivedLen && read == broadcastIdBuffer[0]) {
                receivedLen++;
            }else if(receivedLen > 0) {
                if(read == broadcastIdBuffer[receivedLen]) {
                    receivedLen++;
                }else {
                    receivedLen = 0;
                }
            }

            available--;
            if(receivedLen == 4) {
                timeout = currentMillis + TIMEOUT;
                break;
            }
        }
    }

    /*Handle timeouts*/
    if(!isAwaitingPacket && receivedLen == 4 && currentMillis > timeout) {
        receivedLen = 0;
    }else if(isAwaitingPacket && currentMillis > timeout) {
        isAwaitingPacket = false;
        free(awaitedPacket.buffer);
        awaitedPacket = EMPTY_PACKET;
    }

    /*ReceivedLen = 4 indicates that a info packet has been found*/
    if(!isAwaitingPacket && receivedLen == 4 && available >= sizeof(info_packet_t) - sizeof(int32_t)) {
        uint8_t* packet = (uint8_t*) malloc(sizeof(info_packet_t) - sizeof(int32_t));
        Serial.readBytes(packet, sizeof(info_packet_t) - sizeof(int32_t));

        if(Packets::HandlePacket(packet, sizeof(info_packet_t) - sizeof(int32_t))) {
            timeout = currentMillis + TRANSMISSION_TIME(awaitedPacket.bufferSize) + TIMEOUT;
        }

        available -= sizeof(info_packet_t) - sizeof(int32_t);
        receivedLen = 0;
    }

    /*Waiting for packet*/
    if(isAwaitingPacket && available > 0) {
        int32_t chunkSize = available > bytesLeft ? bytesLeft : available;
        uint8_t* packetChunk = (uint8_t*) malloc(chunkSize);

        Serial.readBytes(packetChunk, chunkSize);
        Packets::HandlePacket(packetChunk, chunkSize);
    }

    /*Send waiting packets*/
    if(!isAwaitingPacket && receivedLen == 0) {
        Handlers::Update();
        HandleSendQueue();
    }
}

/*Packet Sending*/
Packets::Packet* Packets::CreatePacket(uint32_t packetId, void* buffer, uint32_t size) {
    Packets::Packet* pPacket = (Packets::Packet*) malloc(sizeof(Packets::Packet));
    uint8_t* _buffer = (uint8_t*) malloc(size);

    memcpy(_buffer, (void*) buffer, size);

    pPacket->packetId = packetId;
    pPacket->buffer = _buffer;
    pPacket->bufferSize = size;

    return pPacket;
}

Packets::Packet* Packets::CreateMessagePacket(char* message) {
    return Packets::CreatePacket(MESSAGE_ID, (void*) message, strlen(message) * sizeof(char));
}

Packets::Packet* Packets::CreateErrorPacket(char* error) {
    return Packets::CreatePacket(ERROR_ID, (void*) error, strlen(error) * sizeof(char));
}

void Packets::SendInfoPacket(Packet* packet) {
    info_packet_t* info_packet = (info_packet_t*) malloc(sizeof(info_packet_t));
    info_packet->broadcastId = BROADCAST_ID;
    info_packet->packetId = packet->packetId;
    info_packet->packetSize = packet->bufferSize;

    uint8_t* buffer = (uint8_t*) malloc(sizeof(info_packet_t));
    memcpy((void*) buffer, (void*) info_packet, sizeof(info_packet_t));

    Serial.flush(); //Flush in order to send waiting packets
    Serial.write(buffer, sizeof(info_packet_t));
    Serial.flush(); //Force sending of the packet

    /*Free memory to avoid waste*/
    free((void*) buffer);
    free((void*) info_packet);
}

void Packets::SendPacket(Packet* packet) {
    SendInfoPacket(packet);
    Serial.write(packet->buffer, packet->bufferSize);
    Serial.flush(); //Force sending of the packet
}

void Packets::AddPacketToSendQueue(Packets::Packet* packet) {
    /*Create new PacketQueueItem*/
    Packets::PacketQueueItem* pPacketQueueItem = (Packets::PacketQueueItem*) malloc(sizeof(Packets::PacketQueueItem));
    pPacketQueueItem->packet = packet;
    pPacketQueueItem->next = nullptr;

    if(!packetQueue) {
        packetQueue = pPacketQueueItem; //If the queue is empty, make the current packet the first one
        return;
    }

    /*If the queue is not empty, add the current packet to the last one in the queue*/
    Packets::PacketQueueItem* pLastQueueItem = packetQueue;

    while(pLastQueueItem->next != nullptr) {
        pLastQueueItem = pLastQueueItem->next;
    }

    pLastQueueItem->next = pPacketQueueItem;
}

void Packets::HandleSendQueue() {
    while(packetQueue) {
        Packets::PacketQueueItem* pNext = packetQueue->next;    //Look for following packet
        SendPacket(packetQueue->packet);                        //Send the current packet

        /*Free the memory as much as possible*/
        free(packetQueue->packet->buffer);
        free(packetQueue->packet);
        free(packetQueue);

        packetQueue = pNext;                                    //Put the next packet at the first place
    }
}