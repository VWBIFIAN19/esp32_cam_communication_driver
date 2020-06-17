#ifndef UTILS_H
#define UTILS_H

namespace Utils {
    /**
     * Convert int32 to byte-array
     * 
     * @param value Int32-Value that is going to get converted
     * @return given int32 as byte-array
     */
    uint8_t* GetBufferFromInt32(int32_t value);
}

#endif