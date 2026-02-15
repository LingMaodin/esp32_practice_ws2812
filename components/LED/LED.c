#include <stdio.h>
#include "LED.h"
#include "driver/gpio.h"

void led_init(gpio_num_t gpio_num)
{
    esp_err_t err;
    gpio_config_t gpio={
        .pin_bit_mask = (1ULL << gpio_num),//引脚
        .mode = GPIO_MODE_OUTPUT,//模式为输出
        .pull_up_en=GPIO_PULLUP_ENABLE,//开启上拉
        .pull_down_en=GPIO_PULLDOWN_DISABLE,//不开启下拉
        .intr_type=GPIO_INTR_DISABLE//不使用中断
    };
    err = gpio_config(&gpio);
    if (err!=ESP_OK)//也可以直接ESP_ERROR_CHECK(gpio_config(&gpio)); 
    {
        printf("GPIO Config Failed\n");
    }
}

void led_toggle(gpio_num_t gpio_num)
{
    uint32_t level = gpio_get_level(gpio_num);
    gpio_set_level(gpio_num, !level);
}