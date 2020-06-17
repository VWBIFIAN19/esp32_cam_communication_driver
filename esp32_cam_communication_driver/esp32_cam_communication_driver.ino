#include "src/headers/driver.h"

void setup() {
  ESP32_CAM_DRIVER::Setup();
}

void loop() {
  ESP32_CAM_DRIVER::Update();
}
