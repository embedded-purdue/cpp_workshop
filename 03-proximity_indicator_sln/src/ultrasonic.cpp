#include "ultrasonic.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <algorithm>
#include <cmath>

// Physical constants
static constexpr float SPEED_OF_SOUND_CM_PER_US = 0.0343f;  // cm/microseconds

// Timing constants
static constexpr uint32_t TRIGGER_PULSE_US = 10;        // Trigger pulse duration
static constexpr uint32_t TRIGGER_SETTLE_US = 2;        // Time to settle before trigger
static constexpr uint32_t MEASUREMENT_DELAY_MS = 60;    // Delay between measurements

driver::UltrasonicSensor::UltrasonicSensor(gpio_num_t trigger_pin, 
                                            gpio_num_t echo_pin, 
                                            uint32_t timeout_us)
    : trigger_gpio_(trigger_pin, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
      echo_gpio_(echo_pin, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE),
      timeout_us_(timeout_us)
{
    // Set trigger pin to low initially
    trigger_gpio_.set_low();
}

driver::UltrasonicSensor::Status driver::UltrasonicSensor::measure_distance(float& distance) {
    uint32_t pulse_duration_us;
    
    // Send trigger pulse
    send_trigger_pulse();
    
    // Measure echo pulse duration
    Status status = measure_echo_pulse(pulse_duration_us);
    if (status != Status::Success) {
        return status;
    }
    
    // Convert pulse duration to distance in centimeters
    distance = pulse_to_distance(pulse_duration_us);
    
    return Status::Success;
}

driver::UltrasonicSensor::Status driver::UltrasonicSensor::measure_distance_avg(float& distance, 
                                                                             uint8_t samples) {
    if (samples == 0) {
        return Status::Error;
    }
    
    float sum = 0.0f;
    uint8_t valid_samples = 0;
    
    for (uint8_t i = 0; i < samples; ++i) {
        float single_distance;
        Status status = measure_distance(single_distance);
        
        if (status == Status::Success) {
            sum += single_distance;
            valid_samples++;
        }
        
        // Small delay between measurements
        if (i < samples - 1) {
            vTaskDelay(pdMS_TO_TICKS(MEASUREMENT_DELAY_MS));
        }
    }
    
    if (valid_samples == 0) {
        return Status::Error;
    }
    
    distance = sum / valid_samples;
    return Status::Success;
}

bool driver::UltrasonicSensor::is_object_in_range(float threshold_distance) {
    float measured_distance;
    Status status = measure_distance(measured_distance);
    
    if (status != Status::Success) {
        return false;
    }
    
    return measured_distance <= threshold_distance;
}

void driver::UltrasonicSensor::set_timeout(uint32_t timeout_us) {
    timeout_us_ = timeout_us;
}

uint32_t driver::UltrasonicSensor::get_timeout() const {
    return timeout_us_;
}

void driver::UltrasonicSensor::send_trigger_pulse() {
    // Ensure trigger is low
    trigger_gpio_.set_low();
    esp_rom_delay_us(TRIGGER_SETTLE_US);

    // Send 10 microsecond high pulse
    trigger_gpio_.set_high();
    esp_rom_delay_us(TRIGGER_PULSE_US);
    trigger_gpio_.set_low();
}

driver::UltrasonicSensor::Status driver::UltrasonicSensor::measure_echo_pulse(uint32_t& pulse_duration_us) {
    uint64_t start_time, end_time;
    uint64_t timeout_start = esp_timer_get_time();
    
    // Wait for echo pin to go high (start of echo)
    while (!echo_gpio_.read()) {
        if ((esp_timer_get_time() - timeout_start) > timeout_us_) {
            return Status::Timeout;
        }
    }
    start_time = esp_timer_get_time();
    
    // Wait for echo pin to go low (end of echo)
    while (echo_gpio_.read()) {
        if ((esp_timer_get_time() - timeout_start) > timeout_us_) {
            return Status::Timeout;
        }
    }
    end_time = esp_timer_get_time();
    
    pulse_duration_us = static_cast<uint32_t>(end_time - start_time);
    
    // Sanity check: pulse should be reasonable duration
    if (pulse_duration_us < 150 || pulse_duration_us > timeout_us_) {
        return Status::OutOfRange;
    }
    
    return Status::Success;
}

float driver::UltrasonicSensor::pulse_to_distance(uint32_t pulse_duration_us) {
    // Distance = (pulse_duration * speed_of_sound) / 2
    // Divide by 2 because sound travels to object and back
    // Returns distance in centimeters
    
    return (pulse_duration_us * SPEED_OF_SOUND_CM_PER_US) / 2.0f;
}
