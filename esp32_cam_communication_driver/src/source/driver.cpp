#include <cinttypes>
#include "Arduino.h"
#include "../headers/packet_definitions.h"

#include "../headers/definitions.h"
#include "../headers/handlers.h"
#include "../headers/packets.h"
#include "../headers/driver.h"
#include "../headers/utils.h"

void ESP32_CAM_DRIVER::Setup() {
    Serial.begin(SERIAL_BPS);
    Handlers::Setup();
}

void ESP32_CAM_DRIVER::Update() {
    Packets::HandleTransmission();
}