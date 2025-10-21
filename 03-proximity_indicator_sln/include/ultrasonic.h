#pragma once
#include "led.h"  // For Gpio class
#include "esp_timer.h"
#include <cstdint>

namespace driver {

class UltrasonicSensor {
public:
    enum class Status {
        Success,        // Measurement completed successfully
        Timeout,        // No echo received within timeout period
        OutOfRange,     // Object too close or too far
        Error           // General error occurred
    };

    UltrasonicSensor(gpio_num_t trigger_pin, 
                     gpio_num_t echo_pin, 
                     uint32_t timeout_us = 30000);

    ~UltrasonicSensor() = default;

    // Perform a single distance measurement in cm
    Status measure_distance(float& distance);

    // Perform multiple measurements and return the average in centimeters
    Status measure_distance_avg(float& distance, uint8_t samples = 3);

    // Check if an object is within a specified range in centimeters
    bool is_object_in_range(float threshold_distance);

    // Set the measurement timeout
    void set_timeout(uint32_t timeout_us);

    // Get the current timeout setting
    uint32_t get_timeout() const;


private:
    Gpio trigger_gpio_;     // GPIO object for trigger signal
    Gpio echo_gpio_;        // GPIO object for echo signal
    uint32_t timeout_us_;   // Timeout for echo reception in microseconds

    // Send trigger pulse to start measurement
    void send_trigger_pulse();

    // Wait for echo signal and measure duration
    Status measure_echo_pulse(uint32_t& pulse_duration_us);

    // Convert pulse duration to distance in cm
    float pulse_to_distance(uint32_t pulse_duration_us);
};

} // namespace driver
