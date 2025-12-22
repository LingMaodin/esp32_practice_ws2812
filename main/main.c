#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "LED.h"
#include "led_strip.h"
#include <math.h>

void gpio_init()
{
    esp_err_t err;
    gpio_config_t gpio={
        .pin_bit_mask = (1ULL << GPIO_NUM_0),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en=GPIO_PULLUP_ENABLE,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,
        .intr_type=GPIO_INTR_DISABLE
    };
    err = gpio_config(&gpio);
    if (err!=ESP_OK)
    {
        printf("GPIO Config Failed\n");
    }
}
led_strip_handle_t led_strip;
void RGB_init()
{
    led_strip_config_t RGB =
        {
            .strip_gpio_num = GPIO_NUM_48,
            .max_leds = 1,
            .led_model = LED_MODEL_WS2812,
            .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        };
    led_strip_rmt_config_t rmt = {};
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&RGB, &rmt, &led_strip));
}

void app_main(void)
{
    int hue=0;
    RGB_init();
    gpio_init();
    while (1)
    {
        uint8_t red=0,green=0,blue=0;
        if (gpio_get_level(GPIO_NUM_0)==1)
        {
            if (hue<85)
            {
                red=85-hue;
                green=hue;
                blue=0;
            }
            else if (hue<170)
            {
                red=0;
                green=170-hue;
                blue=hue-85;
            }
            else
            {
                red=hue-170;
                green=0;
                blue=255-hue;
            }
            hue++;
            led_strip_set_pixel(led_strip, 0, red*3, green*3, blue*3);
            led_strip_refresh(led_strip);
            vTaskDelay(pdMS_TO_TICKS(20));
            if (hue>=255)
                hue=0;
        }
        else
        {
            led_strip_set_pixel(led_strip, 0, 30, 30, 30);
            led_strip_refresh(led_strip);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}