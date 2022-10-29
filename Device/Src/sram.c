/**
 * @author: Pomin
 * @date: 2022-10-24 20:43:41
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-10-29 17:11:06
 * @description:
 **/
#include "sram.h"

#include <stdio.h>

#define EXT_SRAM_ADDR ((uint32_t)0x68000000)
#define EXT_SRAM_SIZE (1 * 1024 * 1024)

/*
*********************************************************************************************************
*	函 数 名: bsp_TestExtSRAM
*	功能说明: 扫描测试外部SRAM的全部单元。
*	形    参: 无
*	返 回 值: 0 表示测试通过； 大于0表示错误单元的个数。
*********************************************************************************************************
*/
uint32_t bsp_test_sram(void) {
    uint32_t i;
    uint32_t *pSRAM;
    uint8_t *pBytes;
    uint32_t err;
    const uint8_t ByteBuf[4] = {0x55, 0xA5, 0x5A, 0xAA};

    /* 写SRAM */
    pSRAM = (uint32_t *)EXT_SRAM_ADDR;
    for (i = 0; i < EXT_SRAM_SIZE / 4; i++) {
        *pSRAM++ = i;
    }

    /* 读SRAM */
    err = 0;
    pSRAM = (uint32_t *)EXT_SRAM_ADDR;
    for (i = 0; i < EXT_SRAM_SIZE / 4; i++) {
        if (*pSRAM++ != i) {
            err++;
        }
    }
    printf("SDRAM check round 1 error = %u\r\n", err);
    if (err > 0) {
        return (4 * err);
    }

#if 1
    /* 对SRAM 的数据求反并写入 */
    pSRAM = (uint32_t *)EXT_SRAM_ADDR;
    for (i = 0; i < EXT_SRAM_SIZE / 4; i++) {
        *pSRAM = ~*pSRAM;
        pSRAM++;
    }

    /* 再次比较SRAM的数据 */
    err = 0;
    pSRAM = (uint32_t *)EXT_SRAM_ADDR;
    for (i = 0; i < EXT_SRAM_SIZE / 4; i++) {
        if (*pSRAM++ != (~i)) {
            err++;
        }
    }

    printf("SDRAM check round 2 error = %u\r\n", err);
    if (err > 0) {
        return (4 * err);
    }
#endif

    /* 测试按字节方式访问, 目的是验证 FSMC_NBL0 、 FSMC_NBL1 口线 */
    pBytes = (uint8_t *)EXT_SRAM_ADDR;
    for (i = 0; i < sizeof(ByteBuf); i++) {
        *pBytes++ = ByteBuf[i];
    }

    /* 比较SRAM的数据 */
    err = 0;
    pBytes = (uint8_t *)EXT_SRAM_ADDR;
    for (i = 0; i < sizeof(ByteBuf); i++) {
        if (*pBytes++ != ByteBuf[i]) {
            err++;
        }
    }
    printf("SDRAM check round 3 error = %u\r\n", err);
    if (err > 0) {
        return err;
    }
    return 0;
}
