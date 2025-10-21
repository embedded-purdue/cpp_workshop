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


// TODO: Implement the MultiColorLed::on() method


// TODO: Implement the MultiColorLed::off() method


// TODO: Implement the MultiColorLed::set_color() method


// TODO: Implement the MultiColorLed::toggle() method

