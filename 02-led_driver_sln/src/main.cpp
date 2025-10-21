#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include <iostream>

// sleep function using FreeRTOS
inline void sleep_ms(int ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}         


extern "C" void app_main() {

    // Create LED objects
    SingleColorLed builtin_led(GPIO_NUM_2, Led::Configuration::CommonAnode);

    // TODO: create a MultiColorLed object for an RGB LED connected to GPIOs 25, 26, and 27
    MultiColorLed rgb_led(GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_27, Led::Configuration::CommonAnode);

    std::cout << "LED Demo Starting..." << std::endl;
    
    while (1) {
        // Blink the built-in LED
        std::cout << "Built-in LED ON" << std::endl;
        builtin_led.on();
        sleep_ms(500);
        
        std::cout << "Built-in LED OFF" << std::endl;
        builtin_led.off();
        sleep_ms(500);

        // Cycle through RGB colors
        std::cout << "RGB LED - Red" << std::endl;
        rgb_led.set_color(true, false, false);
        sleep_ms(1000);
        
        std::cout << "RGB LED - Green" << std::endl;
        rgb_led.set_color(false, true, false);
        sleep_ms(1000);

        std::cout << "RGB LED - Blue" << std::endl;
        rgb_led.set_color(false, false, true);
        sleep_ms(1000);

        std::cout << "RGB LED - Yellow (Red + Green)" << std::endl;
        rgb_led.set_color(true, true, false);
        sleep_ms(1000);

        std::cout << "RGB LED - Purple (Red + Blue)" << std::endl;
        rgb_led.set_color(true, false, true);
        sleep_ms(1000);

        std::cout << "RGB LED - Cyan (Green + Blue)" << std::endl;
        rgb_led.set_color(false, true, true);
        sleep_ms(1000);
        
        std::cout << "All LEDs OFF" << std::endl;
        builtin_led.off();
        rgb_led.off();
        sleep_ms(1000);
    }
}