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

/* 包含系统头文件 */

/* 包含自定义头文件 */
#include "sysclock.h"
static void HSE_ClockStartUpConfiguration(HSE_Clock_TypeDef HSE_Clock,HSE_ClockStartUpTime_TypeDef HSE_ClockStartUpTime);
/* 自定义新类型 */

/* 自定义宏 */
#define DEBUG
/*******************************************************************************
 * 名称: HSE_ClockStartUpConfiguration
 * 功能: 时钟的选择，可以实现外部的时钟切换
 * 形参: HSE_Clock 外部时钟类型的选择
         HSE_ClockStartUpTime 外部时钟起振稳定时间
 * 返回: 无
 * 说明: 如果外部时钟是大于16MHzflash需要插入一个等待周期
         小于或者等于16MHz则不需要
         这些设置写进flash,运行一次即可,在调试的时候运行即可
               
 ******************************************************************************/
static void HSE_ClockStartUpConfiguration(HSE_Clock_TypeDef HSE_Clock,HSE_ClockStartUpTime_TypeDef HSE_ClockStartUpTime)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);/*要对选项字节写操作，必须要先解锁FLASH*/
  while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*等待解锁完毕*/
 /*如果外部时钟是大于16MHz或者小于等于24MHz,flash需要插入一个等待周期，小于或者等于16MHz则不需要*/
  if(HSE_Clock==HSE_24MHz)   
  FLASH_ProgramOptionByte(FLASH_WAIT_STATES_ADDRESS,HSE_Clock);
  FLASH_ProgramOptionByte(HSE_CLOCK_STARTUP_ADDRESS,HSE_ClockStartUpTime);
  /*外部时钟启动稳定周期*/
  FLASH_Lock(FLASH_MEMTYPE_DATA);/*操作完要加锁*/
    
}
/*******************************************************************************
 * 名称: DefaultSystemClockForHSI
 * 功能: 恢复默认HSI功能的设置
 * 形参: 无     
 * 返回: 无
 * 说明: 这些设置写进flash,运行一次即可,在调试的时候运行即可
 ******************************************************************************/
void DefaultSystemClockForHSI(void)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);/*要对选项字节写操作，必须要先解锁FLASH*/
  while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*等待解锁完毕*/
  FLASH_EraseOptionByte(FLASH_WAIT_STATES_ADDRESS);/*恢复HSI时钟*/
  FLASH_EraseOptionByte(HSE_CLOCK_STARTUP_ADDRESS);
  FLASH_Lock(FLASH_MEMTYPE_DATA);/*操作完要加锁*/
}


/*******************************************************************************
 * 名称: Sysclock_Init
 * 功能: 设置系统时钟频率
 * 形参: 无
 * 返回: 无
 * 说明: 时钟选择
         如果选择外部24MHz主时钟的，外部不是24M的话
         把#define HSE_24M  HSE_VALUE注释掉即可,
         如果不需要切换到外部时钟，把#define __HSE_VALUE  HSE_VALUE
         注释掉即可
 ******************************************************************************/
void SystemClock_Init(SystemClock_TypeDef sysclk)
{

   if(sysclk==HSE_Clock)/*选用外部时钟*/
   {
     #ifdef DEBUG
     HSE_ClockStartUpConfiguration(HSE_24MHz,HSECNT_8CLK);
     #endif
     while (!CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE,\
            CLK_CURRENTCLOCKSTATE_DISABLE));
     /*切换到外部时钟,并等待时钟却换成功*/
   }
   else 
   {
     #ifdef DEBUG
     DefaultSystemClockForHSI();
     #endif
     CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
   }

}


/******************* (C) COPYRIGHT 风驰iCreate嵌入式开发工作室 *****END OF FILE****/