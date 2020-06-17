#ifndef DRIVER_H
#define DRIVER_H

namespace ESP32_CAM_DRIVER {

    /**
     * Once called at the start of the driver
     */
    void Setup();

    /**
     * Updates frequently at around 117 kHz -> 117.000 times per second
     */
    void Update();

}

#endif