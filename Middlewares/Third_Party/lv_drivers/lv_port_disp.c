#include "lv_port_disp.h"

#include "lcd.h"

//变量定义
#define COLOR_BUF_SIZE (LV_HOR_RES_MAX * LV_VER_RES_MAX)  //全屏的大小

#ifdef __CC_ARM

static lv_color_t color_buf[COLOR_BUF_SIZE]
    __attribute__((at(0X68000000)));  //分配到外部1MB sram的最起始处

#else
#    ifdef __GNUC__

static lv_color_t color_buf[COLOR_BUF_SIZE]
    __attribute__((section(".ext_sram")));  //分配到外部1MB sram的最起始处
#    endif  // __GNUC__

#endif  // __CC_ARM

//函数申明
static void
disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);

// lvgl显示接口初始化
void lv_port_disp_init(void) {
    static lv_disp_draw_buf_t disp_buf;
    //显示缓冲区初始化
    lv_disp_draw_buf_init(&disp_buf, color_buf, NULL, COLOR_BUF_SIZE);

    //显示驱动默认值初始化
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    //设置屏幕的显示大小,我这里是为了支持正点原子的多个屏幕,采用动态获取的方式
    //如果你是用于实际项目的话,可以不用设置,那么其默认值就是lv_conf.h中LV_HOR_RES_MAX和LV_VER_RES_MAX宏定义的值
    disp_drv.hor_res = lcddev.width;
    disp_drv.ver_res = lcddev.height;

    //注册显示驱动回调
    disp_drv.flush_cb = disp_flush;

    //注册显示缓冲区
    disp_drv.draw_buf = &disp_buf;

    //注册显示驱动到lvgl中
    lv_disp_drv_register(&disp_drv);
}

//把指定区域的显示缓冲区内容写入到屏幕上,你可以使用DMA或者其他的硬件加速器在后台去完成这个操作
//但是在完成之后,你必须得调用lv_disp_flush_ready()
static void disp_flush(lv_disp_drv_t*   disp_drv,
                       const lv_area_t* area,
                       lv_color_t*      color_p) {
    //把指定区域的显示缓冲区内容写入到屏幕
    LCD_Color_Fill(area->x1, area->y1, area->x2, area->y2, (uint16_t*)color_p);
    //最后必须得调用,通知lvgl库你已经flushing拷贝完成了
    lv_disp_flush_ready(disp_drv);
}
