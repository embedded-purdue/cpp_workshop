#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ========================= GPIO =========================

Gpio::Gpio(gpio_num_t pin,
           gpio_mode_t mode,
           gpio_pullup_t pull_up_en,
           gpio_pulldown_t pull_down_en,
           gpio_int_type_t intr_type)
    : pin_(pin)
{
    cfg_.pin_bit_mask = (1ULL << pin_);
    cfg_.mode = mode;
    cfg_.pull_up_en = pull_up_en;
    cfg_.pull_down_en = pull_down_en;
    cfg_.intr_type = intr_type;
    apply_config();
}

void Gpio::apply_config() {
    gpio_config(&cfg_);
}

void Gpio::set_high() { gpio_set_level(pin_, 1); }
void Gpio::set_low()  { gpio_set_level(pin_, 0); }
void Gpio::toggle()   { gpio_set_level(pin_, !gpio_get_level(pin_)); }
bool Gpio::read() const { return gpio_get_level(pin_); }


// ========================= SINGLE-COLOR LED =========================

SingleColorLed::SingleColorLed(gpio_num_t pin, Configuration config)
    : gpio_(pin, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE) {
    config_ = config;
}

void SingleColorLed::on() {
    if (config_ == Configuration::CommonAnode)
        gpio_.set_high();
    else
        gpio_.set_low();
}

void SingleColorLed::off() {
    if (config_ == Configuration::CommonAnode)
        gpio_.set_low();
    else
        gpio_.set_high();
}

void SingleColorLed::toggle() {
    gpio_.toggle();
}


// ========================= MULTI-COLOR LED =========================

// TODO: Implement the MultiColorLed class constructor
MultiColorLed::MultiColorLed(gpio_num_t red_pin, gpio_num_t green_pin, gpio_num_t blue_pin,
                             Configuration config)
    : red_(red_pin, config),
      green_(green_pin, config),
      blue_(blue_pin, config) {
    config_ = config;
}

// TODO: Implement the MultiColorLed::on() method
void MultiColorLed::on() {
    red_.on();
    green_.on();
    blue_.on();
}

// TODO: Implement the MultiColorLed::off() method
void MultiColorLed::off() {
    red_.off();
    green_.off();
    blue_.off();
}

// TODO: Implement the MultiColorLed::set_color() method
void MultiColorLed::set_color(bool red, bool green, bool blue) {
    // Turn off all colors first
    red_.off();
    green_.off();
    blue_.off();
    
    // Turn on requested colors
    if (red)   red_.on();
    if (green) green_.on();
    if (blue)  blue_.on();
}

// TODO: Implement the MultiColorLed::toggle() method
void MultiColorLed::toggle() {
    red_.toggle();
    green_.toggle();
    blue_.toggle();
}
