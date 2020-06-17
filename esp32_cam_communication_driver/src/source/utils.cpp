#include <cinttypes>
#include "Arduino.h"
#include "../headers/utils.h"

uint8_t* Utils::GetBufferFromInt32(int32_t value) {
    uint8_t* buffer = (uint8_t*) malloc(sizeof(int32_t));
    memcpy(buffer, (void*) &value, sizeof(int32_t));

    return buffer;
}