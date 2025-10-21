#pragma once
#include "driver/gpio.h"

namespace driver {

// === GPIO ===
class Gpio {
public:
    // constructor
    Gpio(gpio_num_t pin,
         gpio_mode_t mode = GPIO_MODE_OUTPUT,
         gpio_pullup_t pull_up_en = GPIO_PULLUP_DISABLE,
         gpio_pulldown_t pull_down_en = GPIO_PULLDOWN_DISABLE,
         gpio_int_type_t intr_type = GPIO_INTR_DISABLE);

    // basic operations
    void set_high();
    void set_low();
    void toggle();
    bool read() const;

private:
    gpio_num_t pin_;
    gpio_config_t cfg_;

    void apply_config(); // internal helper for setup
};


// === LED base class ===
class Led {
public:
    // common anode: led connected gpio <-> gnd and turns on when gpio = 1
    // common cathode: led connected gpio <-> vcc and turns on when gpio = 0
    enum class Configuration { CommonAnode, CommonCathode };

    virtual void on()  = 0;
    virtual void off() = 0;
    virtual void toggle() = 0;

    virtual ~Led() = default;

protected:
    Configuration config_;
};


// === single-color led ===
class SingleColorLed : public Led {
public:
    SingleColorLed(gpio_num_t pin, Configuration config = Configuration::CommonCathode);

    void on() override;
    void off() override;
    void toggle() override;

private:
    Gpio gpio_;
};


// === multi-color led (RGB) ===
class MultiColorLed : public Led {
public:
    MultiColorLed(gpio_num_t red_pin, gpio_num_t green_pin, gpio_num_t blue_pin,
                  Configuration config = Configuration::CommonCathode);

    void on() override;
    void off() override;
    void toggle() override;
    void set_color(bool red, bool green, bool blue);

private:
    SingleColorLed red_;
    SingleColorLed green_;
    SingleColorLed blue_;
};

} // namespace driver
