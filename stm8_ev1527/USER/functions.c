/**
  ******************************************************************************
  * @file function.c
  * @brief This file contains some functions that can be used in the demo.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.0.1
  * @date 11/21/2008
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2008 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "functions.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RAM1_START            0x200
#define RAM1_END              0x21F


void Init_GPIO(void)
{
	//RF DATA接收io
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);
    //跳线配置 

		
		
    //继电器输出
    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);
    
    //led
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);
		
    //Beeper
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
    //GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
	
	//GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);	
    //Key
    GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);
}

void Init_Clock(void)
{
    /* Select fCPU = 16MHz */
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1); //选内部16M不分频
	while(CLK->ICKR & CLK_ICKR_HSIRDY == 0); //等待内部时钟备好
    /* For test purpose output Fcpu on MCO pin */
    //CLK_CCOConfig(CLK_OUTPUT_CPU);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
