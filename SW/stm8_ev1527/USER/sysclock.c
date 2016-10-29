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

/* ����ϵͳͷ�ļ� */

/* �����Զ���ͷ�ļ� */
#include "sysclock.h"
static void HSE_ClockStartUpConfiguration(HSE_Clock_TypeDef HSE_Clock,HSE_ClockStartUpTime_TypeDef HSE_ClockStartUpTime);
/* �Զ��������� */

/* �Զ���� */
#define DEBUG
/*******************************************************************************
 * ����: HSE_ClockStartUpConfiguration
 * ����: ʱ�ӵ�ѡ�񣬿���ʵ���ⲿ��ʱ���л�
 * �β�: HSE_Clock �ⲿʱ�����͵�ѡ��
         HSE_ClockStartUpTime �ⲿʱ�������ȶ�ʱ��
 * ����: ��
 * ˵��: ����ⲿʱ���Ǵ���16MHzflash��Ҫ����һ���ȴ�����
         С�ڻ��ߵ���16MHz����Ҫ
         ��Щ����д��flash,����һ�μ���,�ڵ��Ե�ʱ�����м���
               
 ******************************************************************************/
static void HSE_ClockStartUpConfiguration(HSE_Clock_TypeDef HSE_Clock,HSE_ClockStartUpTime_TypeDef HSE_ClockStartUpTime)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);/*Ҫ��ѡ���ֽ�д����������Ҫ�Ƚ���FLASH*/
  while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*�ȴ��������*/
 /*����ⲿʱ���Ǵ���16MHz����С�ڵ���24MHz,flash��Ҫ����һ���ȴ����ڣ�С�ڻ��ߵ���16MHz����Ҫ*/
  if(HSE_Clock==HSE_24MHz)   
  FLASH_ProgramOptionByte(FLASH_WAIT_STATES_ADDRESS,HSE_Clock);
  FLASH_ProgramOptionByte(HSE_CLOCK_STARTUP_ADDRESS,HSE_ClockStartUpTime);
  /*�ⲿʱ�������ȶ�����*/
  FLASH_Lock(FLASH_MEMTYPE_DATA);/*������Ҫ����*/
    
}
/*******************************************************************************
 * ����: DefaultSystemClockForHSI
 * ����: �ָ�Ĭ��HSI���ܵ�����
 * �β�: ��     
 * ����: ��
 * ˵��: ��Щ����д��flash,����һ�μ���,�ڵ��Ե�ʱ�����м���
 ******************************************************************************/
void DefaultSystemClockForHSI(void)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);/*Ҫ��ѡ���ֽ�д����������Ҫ�Ƚ���FLASH*/
  while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*�ȴ��������*/
  FLASH_EraseOptionByte(FLASH_WAIT_STATES_ADDRESS);/*�ָ�HSIʱ��*/
  FLASH_EraseOptionByte(HSE_CLOCK_STARTUP_ADDRESS);
  FLASH_Lock(FLASH_MEMTYPE_DATA);/*������Ҫ����*/
}


/*******************************************************************************
 * ����: Sysclock_Init
 * ����: ����ϵͳʱ��Ƶ��
 * �β�: ��
 * ����: ��
 * ˵��: ʱ��ѡ��
         ���ѡ���ⲿ24MHz��ʱ�ӵģ��ⲿ����24M�Ļ�
         ��#define HSE_24M  HSE_VALUEע�͵�����,
         �������Ҫ�л����ⲿʱ�ӣ���#define __HSE_VALUE  HSE_VALUE
         ע�͵�����
 ******************************************************************************/
void SystemClock_Init(SystemClock_TypeDef sysclk)
{

   if(sysclk==HSE_Clock)/*ѡ���ⲿʱ��*/
   {
     #ifdef DEBUG
     HSE_ClockStartUpConfiguration(HSE_24MHz,HSECNT_8CLK);
     #endif
     while (!CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE,\
            CLK_CURRENTCLOCKSTATE_DISABLE));
     /*�л����ⲿʱ��,���ȴ�ʱ��ȴ���ɹ�*/
   }
   else 
   {
     #ifdef DEBUG
     DefaultSystemClockForHSI();
     #endif
     CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
   }

}


/******************* (C) COPYRIGHT ���iCreateǶ��ʽ���������� *****END OF FILE****/