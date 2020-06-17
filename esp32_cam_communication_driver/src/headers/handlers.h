#ifndef HANDLERS_H
#define HANDLERS_H

namespace Handlers {
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