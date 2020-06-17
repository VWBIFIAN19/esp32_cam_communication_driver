#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define SERIAL_BPS              115200              //Default BPS = 115200

#define BROADCAST_ID            133769420           //Default Broadcast id = 133769420, it is used to identify packets (usually no need to change this)
#define MESSAGE_ID              691337420           //Default Message id = 691337420, used to identify messages (usually only sent and no need to change this)
#define ERROR_ID                420691337           //Default Error id = 420691337, it is used to identify erros (usually no need to change this)

#define MAX_BYTES_TRANSFERRED   64                  //Number of bytes that can be received at once
#define MAX_PACKET_SIZE         0xFFFFFFFF          //Max number of bytes in a receiving packet -> packets cannot exceed this limit

#endif