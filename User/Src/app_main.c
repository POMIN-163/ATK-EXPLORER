/**
 * @author: Pomin
 * @date: 2022-10-24 16:28:30
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-10-29 17:15:50
 * @description:
 **/
#include "app_main.h"

#include <stdio.h>

#include "cmsis_os.h"
#include "lcd.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "main.h"
#include "rng.h"
#include "sram.h"
#include "tim.h"
#include "touch.h"
#include "usart.h"

char uart1_buff[512];
int uart1_len;
#define print_uart1(format, ...)                            \
    uart1_len = sprintf(uart1_buff, format, ##__VA_ARGS__); \
    HAL_UART_Transmit(&huart1, (uint8_t *)uart1_buff, uart1_len, 0xFFFF);

int app_init(void) {
    HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
    HAL_TIM_Base_Start_IT(&htim13);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    return 0;
}

int app_main(void) {
    app_init();

    __HAL_TIM_SetCompare(&htim14, TIM_CHANNEL_1, 100);

    LCD_Init();

    tp_dev.init();  // 触摸屏初始化
    lv_init();      // lvgl初始化
    lv_port_disp_init();
    lv_port_indev_init();

    lv_example_btn_1();

    if (bsp_test_sram()) {
    } else {
        print_uart1("ok\r\n");
    }

    while (1) {
        tp_dev.scan(0);
        lv_task_handler();
        // HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        // uint32_t rng = HAL_RNG_GetRandomNumber(&hrng);
        // print_uart1(":%d\r\n", rng);
        // print_uart1("lcd:%x\r\n", lcddev.id);
    }

    return 0;
}

#ifdef __CC_ARM
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
#endif

#ifdef __GNUC__
int _write(int fd, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFFFF);
    return len;
}
#endif
