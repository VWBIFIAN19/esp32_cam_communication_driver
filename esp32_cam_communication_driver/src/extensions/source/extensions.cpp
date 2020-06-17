#include "../../headers/extensions.h"

void Extensions::Setup() {
    /*Add your custom setup code here*/
}

void Extensions::Update() {
    /*Add your custom update code here*/
}

void* Extensions::CreatePacketHandler(Packets::Packet* packet) {
    switch (packet->packetId)
    {
        /*Add your custom packets here*/
    }

    return nullptr;
}