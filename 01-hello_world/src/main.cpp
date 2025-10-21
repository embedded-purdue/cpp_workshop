#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <iostream>

extern "C" void app_main() {
    while (1) {
        std::cout << "Hello, World!" << std::endl;
        vTaskDelay(pdMS_TO_TICKS(1000));  // delay about 1 second
    }
}