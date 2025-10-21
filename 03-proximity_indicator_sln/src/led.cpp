#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ========================= GPIO =========================

driver::Gpio::Gpio(gpio_num_t pin,
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

void driver::Gpio::apply_config() {
    gpio_config(&cfg_);
}

void driver::Gpio::set_high() { gpio_set_level(pin_, 1); }
void driver::Gpio::set_low()  { gpio_set_level(pin_, 0); }
void driver::Gpio::toggle()   { gpio_set_level(pin_, !gpio_get_level(pin_)); }
bool driver::Gpio::read() const { return gpio_get_level(pin_); }


// ========================= SINGLE-COLOR LED =========================

driver::SingleColorLed::SingleColorLed(gpio_num_t pin, Configuration config)
    : gpio_(pin, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE) {
    config_ = config;
}

void driver::SingleColorLed::on() {
    if (config_ == Configuration::CommonCathode)
        gpio_.set_high();
    else
        gpio_.set_low();
}

void driver::SingleColorLed::off() {
    if (config_ == Configuration::CommonCathode)
        gpio_.set_low();
    else
        gpio_.set_high();
}

void driver::SingleColorLed::toggle() {
    gpio_.toggle();
}


// ========================= MULTI-COLOR LED =========================
driver::MultiColorLed::MultiColorLed(gpio_num_t red_pin, gpio_num_t green_pin, gpio_num_t blue_pin,
                                      Configuration config)
    : red_(red_pin, config),
      green_(green_pin, config),
      blue_(blue_pin, config) {
    config_ = config;
}

void driver::MultiColorLed::on() {
    red_.on();
    green_.on();
    blue_.on();
}

void driver::MultiColorLed::off() {
    red_.off();
    green_.off();
    blue_.off();
}

void driver::MultiColorLed::set_color(bool red, bool green, bool blue) {
    // Turn off all colors first
    red_.off();
    green_.off();
    blue_.off();
    
    // Turn on requested colors
    if (red)   red_.on();
    if (green) green_.on();
    if (blue)  blue_.on();
}

void driver::MultiColorLed::toggle() {
    red_.toggle();
    green_.toggle();
    blue_.toggle();
}
