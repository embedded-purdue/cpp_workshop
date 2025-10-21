#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "ultrasonic.h"
#include <iostream>
#include <deque>

namespace app {

inline void sleep_ms(int ms) { vTaskDelay(pdMS_TO_TICKS(ms)); }

class ProximityLightingController {
public:

    // Configuration structure
    // You can have a struct within a class!
    // This is a good way to group related settings.
    // I chose to do it here to keep the main controller class cleaner.
    struct Config {
        float warn_threshold_cm;
        float danger_threshold_cm;
        float safe_threshold_cm;
        int update_rate_ms;
        size_t history_size;
              
        // parameterized constructor
        // TODO: write the constructor for Config that takes in the arguments as in the main function,
        // and assigns them to the member variables.
        Config(float warn, float danger, float safe, int rate, size_t hist_size)
            : warn_threshold_cm(warn), danger_threshold_cm(danger), 
              safe_threshold_cm(safe), update_rate_ms(rate),
              history_size(hist_size) {}
    };

    // Constructor for the controller
    ProximityLightingController(driver::MultiColorLed& led, driver::UltrasonicSensor& sensor, const Config& cfg)
        : led_(led), sensor_(sensor), cfg_(cfg) {}

    void run() {
        std::cout << "Starting ProximityLightingController..." << std::endl;
        print_configuration();

        while (true) {
            float distance_cm{};

            // TODO: call the measure_distance method and store the result in a variable named status
            auto status = sensor_.measure_distance(distance_cm);

            if (status != driver::UltrasonicSensor::Status::Success) {
                // Set error color (purple)
                led_.set_color(true, false, true);
                sleep_ms(cfg_.update_rate_ms);
                continue;
            }

            // Store measurement in history
            // TODO: push the distance_cm to the distance_history_ deque
            // if, after pushing, the size exceeds cfg_.history_size, remove the oldest entry
            // hint: you may need to google what the dequeue is and what methods to use
            distance_history_.push_back(distance_cm);
            if (distance_history_.size() > cfg_.history_size) {
                distance_history_.pop_front();
            }

            // Set LED color based on distance
            set_led_for_distance(distance_cm);

            // Diagnostics
            print_diagnostics(distance_cm);

            sleep_ms(cfg_.update_rate_ms);
        }
    }

private:
    driver::MultiColorLed& led_;
    driver::UltrasonicSensor& sensor_;
    Config cfg_;
    
    // STL container for distance history
    std::deque<float> distance_history_;

    void set_led_for_distance(float distance) {
        if (distance < cfg_.danger_threshold_cm) {
            led_.set_color(true, false, false);  // Red - danger
        } else if (distance < cfg_.warn_threshold_cm) {
            led_.set_color(true, true, false);   // Yellow - warning
        } else if (distance < cfg_.safe_threshold_cm) {
            led_.set_color(false, true, false);  // Green - safe
        } else {
            led_.set_color(false, false, true);  // Blue - clear
        }
    }

    const char* get_distance_description(float distance) {
        if (distance < cfg_.danger_threshold_cm) return "Danger";
        if (distance < cfg_.warn_threshold_cm) return "Warning";
        if (distance < cfg_.safe_threshold_cm) return "Safe";
        return "Clear";
    }

    void print_diagnostics(float distance) {
        std::cout << "Distance:" << distance << " cm | " << get_distance_description(distance) << std::endl;
    }

    void print_configuration() {
        std::cout << "=== Configuration ===" << std::endl;
        std::cout << "Danger threshold: " << cfg_.danger_threshold_cm << " cm" << std::endl;
        std::cout << "Warning threshold: " << cfg_.warn_threshold_cm << " cm" << std::endl;
        std::cout << "Safe threshold: " << cfg_.safe_threshold_cm << " cm" << std::endl;
        std::cout << "Update rate: " << cfg_.update_rate_ms << " ms" << std::endl;
        std::cout << "History size: " << cfg_.history_size << std::endl;
        std::cout << "===================" << std::endl;
    }
};

} // namespace app


extern "C" void app_main() {
    // Initialize RGB LED (Red: GPIO27, Green: GPIO26, Blue: GPIO25)
    driver::MultiColorLed led(GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_25, driver::Led::Configuration::CommonAnode);

    // Initialize Ultrasonic Sensor (Trigger: GPIO16, Echo: GPIO17)
    driver::UltrasonicSensor sensor(GPIO_NUM_16, GPIO_NUM_17);
    
    // Simple configuration for proximity detection
    app::ProximityLightingController::Config config(
        20.0f,  // warn_threshold_cm
        10.0f,  // danger_threshold_cm 
        50.0f,  // safe_threshold_cm
        200,    // update_rate_ms
        8       // history_size
    );

    app::ProximityLightingController controller(led, sensor, config);
    controller.run();
}