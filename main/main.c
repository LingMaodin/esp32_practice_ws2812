#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led_strip.h"
#define GPIO_RGB GPIO_NUM_48
#define GPIO_BUTTON GPIO_NUM_0
static const uint16_t breath_table[]={// 呼吸表
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  2,  3,  5,  7,  9, 12,
    16, 21, 27, 35, 43, 53, 65, 79, 95,113,133,156,182,211,244,279,
    319,363,410,463,520,581,648,721,798,882,972,1067,1169,1278,1393,1516,
    1645,1782,1926,2077,2236,2403,2577,2759,2949,3147,3353,3567,3789,4019,4256,4501,
    4754,5015,5283,5558,5840,6129,6425,6728,7037,7351,7672,7998,8329,8665,9005,9350,
    9698,10049,10404,10761,11120,11480,11842,12205,12567,12930,13292,13652,14011,14368,14722,15072,      
    15419,15762,16100,16432,16759,17080,17393,17700,17999,18290,18572,18844,19108,19361,19604,19836,     
    20058,20267,20465,20651,20824,20984,21131,21265,21386,21493,21586,21665,21729,21780,21816,21838,     
    21845,21838,21816,21780,21729,21665,21586,21493,21386,21265,21131,20984,20824,20651,20465,20267,     
    20058,19836,19604,19361,19108,18844,18572,18290,17999,17700,17393,17080,16759,16432,16100,15762,     
    15419,15072,14722,14368,14011,13652,13292,12930,12567,12205,11842,11480,11120,10761,10404,10049,     
    9698,9350,9005,8665,8329,7998,7672,7351,7037,6728,6425,6129,5840,5558,5283,5015,
    4754,4501,4256,4019,3789,3567,3353,3147,2949,2759,2577,2403,2236,2077,1926,1782,
    1645,1516,1393,1278,1169,1067,972,882,798,721,648,581,520,463,410,363,
    319,279,244,211,182,156,133,113, 95, 79, 65, 53, 43, 35, 27, 21,
    16, 12,  9,  7,  5,  3,  2,  1,  1,  0,  0,  0,  0,  0,  0,  0,
};

led_strip_handle_t led_strip;// LED句柄
static uint16_t hue = 0;// 色相初始值

void button_init()// 按键初始化
{
    gpio_config_t gpio = {
        .pin_bit_mask = (1ULL << GPIO_BUTTON),  // 初始化引脚
        .mode = GPIO_MODE_INPUT,               // 模式
        .pull_up_en = GPIO_PULLUP_ENABLE,      // 上拉
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // 下拉
        .intr_type = GPIO_INTR_DISABLE         // 中断
    };
    ESP_ERROR_CHECK(gpio_config(&gpio)); // 是否成功判断
}

void RGB_init()// RGB灯珠初始化
{
    led_strip_config_t RGB =
        {
            .strip_gpio_num = GPIO_RGB,                               // 连接引脚
            .max_leds = 1,                                               // 灯珠数量
            .led_model = LED_MODEL_WS2812,                               // 灯珠型号
            .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB, // 传输颜色顺序
        };
    led_strip_rmt_config_t rmt = {};                                   // RMT（红外遥控）时钟配置
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&RGB, &rmt, &led_strip)); // 是否成功判断
    led_strip_clear(led_strip);// 清除灯珠显示
}

void breath()// 呼吸灯
{
    static uint8_t steps=0,value=0;
    value=breath_table[steps]/256;//亮度归一化=呼吸表值/256
    led_strip_set_pixel_hsv(led_strip, 0, hue, 255, value);
    led_strip_refresh(led_strip);
    vTaskDelay(pdMS_TO_TICKS(20));//延时20ms
    steps++;
}

void rainbow() // 彩虹渐变
{
    led_strip_set_pixel_hsv(led_strip, 0, hue, 255, 85);//设置HSV颜色
    led_strip_refresh(led_strip);//刷新显示
    vTaskDelay(pdMS_TO_TICKS(20));//延时20ms
    hue++;
    if(hue>=360) hue=0;//色相循环
}

void app_main(void)
{
    RGB_init();
    button_init();
    while (1)
    {
        if (gpio_get_level(GPIO_BUTTON) == 1)//常态呼吸灯
        {
            breath();
        }
        else//按下彩虹渐变
        {
            rainbow();
        }
    }
}
