/* Includes ------------------------------------------------------------------*/
#include "stm8s_tim1.h"
#include "stm8s.h"
#include "sysclock.h"
#include "functions.h" 
#include "e2prom.h"
#include "head.h"

unsigned int ii = 0, loop = 0, loopval = 0, kk = 0, Key_count = 0;
u32 ttt = 0, jj = 0;
u32 longt = 0;

u32 Get1527_Data(void)
{
	unsigned int temp = 0;
	u32 dat = 0;
		
	dat = 0;
	
	TIM1_SetCounter(0);
	TIM1_Cmd(ENABLE);
	//找第一组同步码
	do {
		temp = TIM1_GetCounter();
	} while (GPIO_ReadInputPin(GPIOB, GPIO_PIN_5) == 0x00 && temp <= 16000);
	TIM1_Cmd(DISABLE);
	if (temp > 16000) {
		return 0;//长时间无信号,失败
	}
	
	TIM1_SetCounter(0);
	TIM1_Cmd(ENABLE);
	do {
		temp = TIM1_GetCounter();
	} while (GPIO_ReadInputPin(GPIOB, GPIO_PIN_5) == 0x20 && temp <= 3000);
	TIM1_Cmd(DISABLE);
	if (temp > 3000) {
		return 0;//长时间无信号,失败
	}
	
	TIM1_SetCounter(0);
	TIM1_Cmd(ENABLE);
	do {
		temp = TIM1_GetCounter();
		longt = temp;
	} while (GPIO_ReadInputPin(GPIOB, GPIO_PIN_5) == 0x00 && temp <= 15000);
	TIM1_Cmd(DISABLE);
	if (temp > 15000 || temp < 10000) {
		return 0;
	}
	
	//数据段
	for (ii = 0; ii < 24; ii++) {
		TIM1_SetCounter(0);
		TIM1_Cmd(ENABLE);
		do {
			temp = TIM1_GetCounter();
		} while (GPIO_ReadInputPin(GPIOB, GPIO_PIN_5) == 0x20 && temp < 2050);

		TIM1_Cmd(DISABLE);
        
        
        /*if (temp > 250 && temp < 500) { //窄波
          dat = dat << 1;
        } else if (temp > 800 && temp < 2050) { //宽波
          dat = dat << 1;
          dat++;
        } else {
          return 0;
        }*/
        
		
        /*动态分析解数据码*/
		if ((temp > (longt / 128)) && (temp < (longt / 16))) { //窄波
                        dat = dat << 1;
		} else if ((temp > (longt / 16)) && (temp < (longt / 8))) { //宽波	
                        dat = dat << 1;
			dat++;
                        
		} else {
			return 0;
		}
	
		//数据段中高电平后的低电平
		TIM1_SetCounter(0);
		TIM1_Cmd(ENABLE);
		do {
			temp = TIM1_GetCounter();
			if (temp > 2050) {//长时间无信号，失败
				break;
			}
		} while (GPIO_ReadInputPin(GPIOB, GPIO_PIN_5) == 0x00 & temp < 2050);
	  
		TIM1_Cmd(DISABLE);
		if (temp >= 2050) {
			return 0;
		}
	}
	return dat;
}


void main()
{
  
	Init_Clock();//初始化时钟
	Init_GPIO();//初始化gpio
        
	GPIO_WriteLow(GPIOC, GPIO_PIN_3); //关蜂鸣
	GPIO_WriteHigh(GPIOD, GPIO_PIN_4); //关LED
        
	TIM1_DeInit(); //TIM1初始化
	TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 20000, 0);//20ms定时器,内部晶振,不分频,15999
	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
	//开启中断
	enableInterrupts();	
	
	/* Define FLASH programming time */
	FLASH_DeInit();
	FLASH_Unlock(FLASH_MEMTYPE_DATA); //解锁eeprom
	FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
	Data_count = FLASH_ReadByte(0x4000); //读取遥控器个数
	
	/*上电使得TIM1活动下*/
	TIM1_Cmd(ENABLE);
	for(ii = 0; ii < 60000; ii++);
	TIM1_Cmd(DISABLE);
	
	while (1) {
		//遥控器学习或遥控器删除时的按键处理
		if (GPIO_ReadInputPin(GPIOC, GPIO_PIN_6) == 0x00) { 
			TIM1_Cmd(ENABLE);
			while (GPIO_ReadInputPin(GPIOC, GPIO_PIN_6) == 0x00) {
				//如果按下时间超过7s
				if (Time_Key_Press > 250) {
					Time_Key_Press = 251;
					TIM1_Cmd(DISABLE);
					ttt = E2PROM_EraseAll();
					Data_count = 0;

					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//蜂鸣
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteLow(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//蜂鸣
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//蜂鸣
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteLow(GPIOD,GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//蜂鸣
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//蜂鸣
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteLow(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//蜂鸣
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					break;
				} else if (Time_Key_Press > 100) { //如果按键大于4s
					GPIO_WriteLow(GPIOD, GPIO_PIN_4);
				}
			}

			while (GPIO_ReadInputPin(GPIOC, GPIO_PIN_6) == 0x00);
			TIM1_Cmd(DISABLE);
			if(Time_Key_Press > 100 && Time_Key_Press <= 250) {
				//遥控器学习
				ttt = 0;
				while (ttt == 0) {
					ttt = Get1527_Data();
				}
				ttt = E2PROM_WriteOne(ttt);
				Data_count++; //遥控器数量增加1

				for(ii = 0; ii < 60000; ii++);
				GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
				GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//蜂鸣
				for(ii = 0; ii < 60000; ii++);
				GPIO_WriteLow(GPIOD, GPIO_PIN_4);
				GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//蜂鸣
				for(ii = 0; ii < 60000; ii++);
				GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
				GPIO_WriteHigh(GPIOD, GPIO_PIN_4);//学习完之后，指示灯灭，回到常态
			}
			Time_Key_Press=0;
		}
	//遥控器学习或遥控器删除时的按键处理--END
	//工作模式的确定--END
		do {
			kk = 0;
			ttt = Get1527_Data(); //获取内码
			Data_count = FLASH_ReadByte(0x4000); //读取遥控器数
			if (ttt != 0 && Data_count != 0) { //如果获取内码不为0, 遥控器数量也不为0	
				//得到遥控器数值之后的处理 		  			
				for (ii = 0; ii < Data_count; ii++) { //在eeprom里遍历查找内码
					jj = E2PROM_ReadU32(0x4001 + ii * 3);
					if ((ttt & 0xfffff0) == (jj & 0xfffff0)) { //内码密码已学习
						kk = 1;
						break;
					}				
				}
					
				if(kk == 1) {
					kk = 0;
					jj = ttt & 0x00000f; //取后四位数据位
					if (jj == 1) { //if KEY1
						GPIO_WriteHigh(GPIOC, GPIO_PIN_7); //吸合继电器
                                                GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					} else if(jj == 2) { //if KEY2
						GPIO_WriteLow(GPIOC, GPIO_PIN_7); //释放继电器
                                                GPIO_WriteLow(GPIOD, GPIO_PIN_4);
					}
					//else if(jj==4)GPIO_WriteReverse(GPIOC,GPIO_PIN_7);//A
					//else if(jj==8)GPIO_WriteReverse(GPIOC,GPIO_PIN_7);//B
					
					if(kk == 0) {
						for (ii = 0; ii < 60000; ii++);
						for (ii = 0; ii < 60000; ii++);
						for (ii = 0; ii < 60000; ii++);
					}
				}
			}
		} while (kk == 1);
	}
}

void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
	while (1)
	{
	}
}