#include "touch.h"

#include "lcd.h"
#include "math.h"
#include "stdlib.h"

static void delay_us(int us) {
    for (size_t i = 0; i < us; i++) {
        for (size_t j = 0; j < 168000; j++) {
        }
    }
}

char *str =
    "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
    "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
    "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh";

_m_tp_dev tp_dev = {
    TP_Init, TP_Scan,
};

char aa[] = {};

// 默认为touchtype=0的数据.
uint8_t CMD_RDX = 0XD0;
uint8_t CMD_RDY = 0X90;

// SPI写数据
// 向触摸屏IC写入1byte数据
// num:要写入的数据
void TP_Write_Byte(uint8_t num) {
    uint8_t count = 0;
    for (count = 0; count < 8; count++) {
        if (num & 0x80)
            TDIN = 1;
        else
            TDIN = 0;
        num <<= 1;
        TCLK = 0;
        delay_us(1);
        TCLK = 1;  // 上升沿有效
    }
}

// SPI读数据
// 从触摸屏IC读取adc值
// CMD:指令
// 返回值:读到的数据
uint16_t TP_Read_AD(uint8_t CMD) {
    uint8_t count = 0;
    uint16_t Num = 0;
    TCLK = 0;            // 先拉低时钟
    TDIN = 0;            // 拉低数据线
    TCS = 0;             // 选中触摸屏IC
    TP_Write_Byte(CMD);  // 发送命令字
    delay_us(6);         // ADS7846的转换时间最长为6us
    TCLK = 0;
    delay_us(1);
    TCLK = 1;  // 给1个时钟，清除BUSY
    delay_us(1);
    TCLK = 0;
    for (count = 0; count < 16; count++)  // 读出16位数据,只有高12位有效
    {
        Num <<= 1;
        TCLK = 0;  // 下降沿有效
        delay_us(1);
        TCLK = 1;
        if (DOUT) Num++;
    }
    Num >>= 4;  // 只有高12位有效.
    TCS = 1;    // 释放片选
    return (Num);
}

// 读取一个坐标值(x或者y)
// 连续读取READ_TIMES次数据,对这些数据升序排列,
// 然后去掉最低和最高LOST_VAL个数,取平均值
// xy:指令（CMD_RDX/CMD_RDY）
// 返回值:读到的数据
#define READ_TIMES 5  // 读取次数
#define LOST_VAL   1  // 丢弃值

uint16_t TP_Read_XOY(uint8_t xy) {
    uint16_t i, j;
    uint16_t buf[READ_TIMES];
    uint16_t sum = 0;
    uint16_t temp;
    for (i = 0; i < READ_TIMES; i++) buf[i] = TP_Read_AD(xy);
    for (i = 0; i < READ_TIMES - 1; i++)  // 排序
    {
        for (j = i + 1; j < READ_TIMES; j++) {
            if (buf[i] > buf[j])  // 升序排列
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    sum = 0;
    for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++) sum += buf[i];
    temp = sum / (READ_TIMES - 2 * LOST_VAL);
    return temp;
}

// 读取x,y坐标
// 最小值不能少于100.
// x,y:读取到的坐标值
// 返回值:0,失败;1,成功。
uint8_t TP_Read_XY(uint16_t *x, uint16_t *y) {
    uint16_t xtemp, ytemp;
    xtemp = TP_Read_XOY(CMD_RDX);
    ytemp = TP_Read_XOY(CMD_RDY);
    // if(xtemp<100||ytemp<100)return 0;//读数失败
    *x = xtemp;
    *y = ytemp;
    return 1;  // 读数成功
}

// 连续2次读取触摸屏IC,且这两次的偏差不能超过
// ERR_RANGE,满足条件,则认为读数正确,否则读数错误.
// 该函数能大大提高准确度
// x,y:读取到的坐标值
// 返回值:0,失败;1,成功。
#define ERR_RANGE 50  // 误差范围

uint8_t TP_Read_XY2(uint16_t *x, uint16_t *y) {
    uint16_t x1, y1;
    uint16_t x2, y2;
    uint8_t flag;
    flag = TP_Read_XY(&x1, &y1);
    if (flag == 0) return (0);
    flag = TP_Read_XY(&x2, &y2);
    if (flag == 0) return (0);
    if (((x2 <= x1 && x1 < x2 + ERR_RANGE) ||
         (x1 <= x2 && x2 < x1 + ERR_RANGE))  // 前后两次采样在+-50内
        && ((y2 <= y1 && y1 < y2 + ERR_RANGE) ||
            (y1 <= y2 && y2 < y1 + ERR_RANGE))) {
        *x = (x1 + x2) / 2;
        *y = (y1 + y2) / 2;
        return 1;
    } else
        return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// 与LCD部分有关的函数
// 画一个触摸点
// 用来校准用的
// x,y:坐标
// color:颜色
void TP_Drow_Touch_Point(uint16_t x, uint16_t y, uint16_t color) {
    POINT_COLOR = color;
    LCD_DrawLine(x - 12, y, x + 13, y);  // 横线
    LCD_DrawLine(x, y - 12, x, y + 13);  // 竖线
    LCD_DrawPoint(x + 1, y + 1);
    LCD_DrawPoint(x - 1, y + 1);
    LCD_DrawPoint(x + 1, y - 1);
    LCD_DrawPoint(x - 1, y - 1);
    LCD_Draw_Circle(x, y, 6);  // 画中心圈
}

// 画一个大点(2*2的点)
// x,y:坐标
// color:颜色
void TP_Draw_Big_Point(uint16_t x, uint16_t y, uint16_t color) {
    POINT_COLOR = color;
    LCD_DrawPoint(x, y);  // 中心点
    LCD_DrawPoint(x + 1, y);
    LCD_DrawPoint(x, y + 1);
    LCD_DrawPoint(x + 1, y + 1);
}

//////////////////////////////////////////////////////////////////////////////////
// 触摸按键扫描
// tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
// 返回值:当前触屏状态.
// 0,触屏无触摸;1,触屏有触摸
uint8_t TP_Scan(uint8_t tp) {
    if (PEN == 0)  // 有按键按下
    {
        if (tp)
            TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]);  // 读取物理坐标
        else if (TP_Read_XY2(&tp_dev.x[0],
                             &tp_dev.y[0]))  // 读取屏幕坐标
        {
            tp_dev.x[0] = tp_dev.xfac * tp_dev.x[0] +
                          tp_dev.xoff;  // 将结果转换为屏幕坐标
            tp_dev.y[0] = tp_dev.yfac * tp_dev.y[0] + tp_dev.yoff;
        }
        if ((tp_dev.sta & TP_PRES_DOWN) == 0)  // 之前没有被按下
        {
            tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES;  // 按键按下
            tp_dev.x[4] = tp_dev.x[0];  // 记录第一次按下时的坐标
            tp_dev.y[4] = tp_dev.y[0];
        }
    } else {
        if (tp_dev.sta & TP_PRES_DOWN)  // 之前是被按下的
        {
            tp_dev.sta &= ~(1 << 7);  // 标记按键松开
        } else                        // 之前就没有被按下
        {
            tp_dev.x[4] = 0;
            tp_dev.y[4] = 0;
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
        }
    }
    return tp_dev.sta & TP_PRES_DOWN;  // 返回当前的触屏状态
}

// 触摸屏初始化
// 返回值:0,没有进行校准
// 1,进行过校准
uint8_t TP_Init(void) {
    if (lcddev.id == 0X5510)  // 电容触摸屏
    {
        if (GT9147_Init() == 0)  // 是GT9147
        {
            tp_dev.scan = GT9147_Scan;  // 扫描函数指向GT9147触摸屏扫描
        }
        tp_dev.touchtype |= 0X80;               // 电容屏
        tp_dev.touchtype |= lcddev.dir & 0X01;  // 横屏还是竖屏
        return 0;
    }
    return 1;
}
