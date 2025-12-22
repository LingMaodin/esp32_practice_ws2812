#pragma once
#include <stdint.h>
#include "driver/gpio.h"

void led_init(gpio_num_t gpio_num);
void led_toggle(gpio_num_t gpio_num);