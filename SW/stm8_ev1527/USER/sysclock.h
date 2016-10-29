/******************** (C) COPYRIGHT  风驰iCreate嵌入式开发工作室 **************
 * 文件名  ：sysclock.c
 * 描述    ：系统时钟设置   
 * 实验平台：风驰STM8开发板
 * 库版本  ：V2.1.0
 * 作者    ：风驰  QQ：779814207
 * 博客    ：
 * 淘宝    ：http://shop71177993.taobao.com/
 *修改时间 ：2012-11-9

*******************************************************************************/
#ifndef __SYSCLOCK_H
#define __SYSCLOCK_H

/* 包含系统头文件 */

/* 包含自定义头文件 */
#include "stm8s.h"

/* 自定义数据类型 */

typedef enum{
  ROP_ADDRESS=(uint16_t)0x4800,
  UBC_ADDRESS=(uint16_t)0x4801,
  AFR_ADDRESS=(uint16_t)0x4803,
  WTD_ADDRESS=(uint16_t)0x4805,
  CLK_OPTION_ADDRESS=(uint16_t)0x4807,
  HSE_CLOCK_STARTUP_ADDRESS=(uint16_t)0x4809,
  FLASH_WAIT_STATES_ADDRESS=(uint16_t)0x480D,
  BOOTLOADER_ADDRESS=(uint16_t)0x487E
  
}OptionByte_Address_TypeDef;


typedef enum{
  
  HSI_Clock,
  HSE_Clock
}SystemClock_TypeDef;
typedef enum{
  HSECNT_2048CLK=0x00,
  HSECNT_128CLK=0xb4,
  HSECNT_8CLK=0xd2,
  HSECNT_0_5CLK=0xe1 
}HSE_ClockStartUpTime_TypeDef;
typedef enum{
  
  HSE_16MHz,
  HSE_24MHz=(uint8_t)0x01
}HSE_Clock_TypeDef;
/* 自定义常量宏和表达式宏 */

/* 声明给外部使用的变量 */

/* 声明给外部使用的函数 */

void SystemClock_Init(SystemClock_TypeDef sysclk);
void DefaultSystemClockForHSI(void);
#endif