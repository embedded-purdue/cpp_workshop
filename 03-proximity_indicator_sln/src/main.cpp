#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "ultrasonic.h"
#include <iostream>
#include <iomanip>

// sleep function using FreeRTOS
inline void sleep_ms(int ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

// function to set LED color based on distance
void set_proximity_indicator(MultiColorLed& led, float distance_cm) {
    if (distance_cm < 10.0f) {
        // Very close: Red (danger)
        led.set_color(true, false, false);
    } else if (distance_cm < 20.0f) {
        // Close: Yellow (warning)
        led.set_color(true, true, false);
    } else if (distance_cm < 50.0f) {
        // Medium distance: Green (safe)
        led.set_color(false, true, false);
    } else {
        // Far or no object: Blue (clear)
        led.set_color(false, false, true);
    }
}

extern "C" void app_main() {

    // Initialize RGB LED (Red: GPIO27, Green: GPIO26, Blue: GPIO25)
    MultiColorLed rgb_led(GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_25, Led::Configuration::CommonCathode);
    
    // Initialize Ultrasonic Sensor (Trigger: GPIO16, Echo: GPIO17)
    UltrasonicSensor ultrasonic(GPIO_NUM_16, GPIO_NUM_17);
    
    std::cout << "Proximity Indicator Starting..." << std::endl;
    
    while (true) {
        float distance_cm;
        
        // Measure distance and set LED color
        if (ultrasonic.measure_distance(distance_cm) != UltrasonicSensor::Status::Success) {
            // Measurement failed - set LED to purple (error indication)
            rgb_led.set_color(true, false, true);
            continue;
        }

        // Print distance and set LED color
        std::cout << "Distance: " << distance_cm << " cm" << std::endl;
        set_proximity_indicator(rgb_led, distance_cm);
        
        // Wait before next measurement
        sleep_ms(200);
    }
}