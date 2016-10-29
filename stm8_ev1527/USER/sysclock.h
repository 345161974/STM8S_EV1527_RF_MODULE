/******************** (C) COPYRIGHT  ���iCreateǶ��ʽ���������� **************
 * �ļ���  ��sysclock.c
 * ����    ��ϵͳʱ������   
 * ʵ��ƽ̨�����STM8������
 * ��汾  ��V2.1.0
 * ����    �����  QQ��779814207
 * ����    ��
 * �Ա�    ��http://shop71177993.taobao.com/
 *�޸�ʱ�� ��2012-11-9

*******************************************************************************/
#ifndef __SYSCLOCK_H
#define __SYSCLOCK_H

/* ����ϵͳͷ�ļ� */

/* �����Զ���ͷ�ļ� */
#include "stm8s.h"

/* �Զ����������� */

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
/* �Զ��峣����ͱ��ʽ�� */

/* �������ⲿʹ�õı��� */

/* �������ⲿʹ�õĺ��� */

void SystemClock_Init(SystemClock_TypeDef sysclk);
void DefaultSystemClockForHSI(void);
#endif