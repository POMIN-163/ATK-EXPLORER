/**
 * @author: Pomin
 * @date: 2022-10-29 20:12:56
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-10-29 21:26:57
 * @description: 别名区
 **/
#ifndef _ALIAS_REG_H
#define _ALIAS_REG_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "stm32f407xx.h"

// 位带操作
// 公式：
//      最终地址 ＝ 别名区基址 + ((A ‐ 外设基址)*8 + n)*4 = 别名区基址 + (A ‐ 外设基址)*32 + n*4
// 解释：
//      原地址相对外设基址偏移值膨胀 32 倍得到外设在别名区的基址
//      原 1 bit变为 32bit, + n*4 找到第n个管脚 (*4)是表示四个字节即 32bit

#define MCU_PERIPH_BASE    PERIPH_BASE     // 外设基址
#define MCU_PERIPH_BB_BASE PERIPH_BB_BASE  // 别名区基址

#define MEM_ADDR(addr)    (*((volatile unsigned long *)(addr)))            // 一个数字转为地址并取值
#define DIFF_ADDR(addr)   ((uint32_t)(addr) - (uint32_t)MCU_PERIPH_BASE)   // 地址相对外设基址偏移
#define BIT_BAND(addr)    ((MCU_PERIPH_BB_BASE + (DIFF_ADDR(addr) << 5)))  // 位带膨胀后的基址
#define BIT_REG(reg, bit) ((uint32_t *)BIT_BAND(&(reg)))[bit]              // 算出原来的1bit 膨胀后的地址
// #define BIT_REG(reg, bit) MEM_ADDR((BIT_BAND(&(reg)) + (((uint32_t)(bit)) << 2)))

#define PF ((uint32_t *)BIT_BAND(&(GPIOF->ODR)))  // 此写法可以写成 PF[10] = 1;

#define PAout(n) BIT_REG(GPIOA->ODR, n)  // 此写法可以写成 PAout(1) = 1;
#define PBout(n) BIT_REG(GPIOB->ODR, n)
#define PCout(n) BIT_REG(GPIOC->ODR, n)
#define PDout(n) BIT_REG(GPIOD->ODR, n)
#define PEout(n) BIT_REG(GPIOE->ODR, n)
#define PFout(n) BIT_REG(GPIOF->ODR, n)
#define PGout(n) BIT_REG(GPIOG->ODR, n)

#define PAin(n) BIT_REG(GPIOA->IDR, n)
#define PBin(n) BIT_REG(GPIOB->IDR, n)
#define PCin(n) BIT_REG(GPIOC->IDR, n)
#define PDin(n) BIT_REG(GPIOD->IDR, n)
#define PEin(n) BIT_REG(GPIOE->IDR, n)
#define PFin(n) BIT_REG(GPIOF->IDR, n)
#define PGin(n) BIT_REG(GPIOG->IDR, n)

#ifdef __cplusplus
}
#endif
#endif  // _ALIAS_REG_H
