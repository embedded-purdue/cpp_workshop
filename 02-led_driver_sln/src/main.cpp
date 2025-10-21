#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include <iostream>
#include <array>

// structure to hold color test data
struct ColorTest {
    const char* name;
    bool red;
    bool green;
    bool blue;
};

// define color test sequence
constexpr std::array<ColorTest, 7> color_sequence = {{
    {"Red",                    true,  false, false},
    {"Green",                  false, true,  false},
    {"Blue",                   false, false, true},
    {"Yellow (Red + Green)",   true,  true,  false},
    {"Purple (Red + Blue)",    true,  false, true},
    {"Cyan (Green + Blue)",    false, true,  true},
    {"All LEDs OFF",           false, false, false}
}};

// sleep function using FreeRTOS
inline void sleep_ms(int ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

extern "C" void app_main() {

    MultiColorLed rgb_led(GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_25, Led::Configuration::CommonAnode);

    std::cout << "LED Demo Starting..." << std::endl;
    
    while (true) {

        for (const auto& test : color_sequence) {
            std::cout << "RGB LED - " << test.name << std::endl;
            rgb_led.set_color(test.red, test.green, test.blue);
            sleep_ms(2000);
        }
        
        std::cout << "=== Sequence Complete - Starting Over ===" << std::endl;
        sleep_ms(1000);
    }
}