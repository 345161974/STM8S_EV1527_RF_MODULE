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
	//�ҵ�һ��ͬ����
	do {
		temp = TIM1_GetCounter();
	} while (GPIO_ReadInputPin(GPIOB, GPIO_PIN_5) == 0x00 && temp <= 16000);
	TIM1_Cmd(DISABLE);
	if (temp > 16000) {
		return 0;//��ʱ�����ź�,ʧ��
	}
	
	TIM1_SetCounter(0);
	TIM1_Cmd(ENABLE);
	do {
		temp = TIM1_GetCounter();
	} while (GPIO_ReadInputPin(GPIOB, GPIO_PIN_5) == 0x20 && temp <= 3000);
	TIM1_Cmd(DISABLE);
	if (temp > 3000) {
		return 0;//��ʱ�����ź�,ʧ��
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
	
	//���ݶ�
	for (ii = 0; ii < 24; ii++) {
		TIM1_SetCounter(0);
		TIM1_Cmd(ENABLE);
		do {
			temp = TIM1_GetCounter();
		} while (GPIO_ReadInputPin(GPIOB, GPIO_PIN_5) == 0x20 && temp < 2050);

		TIM1_Cmd(DISABLE);
        
        
        /*if (temp > 250 && temp < 500) { //խ��
          dat = dat << 1;
        } else if (temp > 800 && temp < 2050) { //��
          dat = dat << 1;
          dat++;
        } else {
          return 0;
        }*/
        
		
        /*��̬������������*/
		if ((temp > (longt / 128)) && (temp < (longt / 16))) { //խ��
                        dat = dat << 1;
		} else if ((temp > (longt / 16)) && (temp < (longt / 8))) { //��	
                        dat = dat << 1;
			dat++;
                        
		} else {
			return 0;
		}
	
		//���ݶ��иߵ�ƽ��ĵ͵�ƽ
		TIM1_SetCounter(0);
		TIM1_Cmd(ENABLE);
		do {
			temp = TIM1_GetCounter();
			if (temp > 2050) {//��ʱ�����źţ�ʧ��
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
  
	Init_Clock();//��ʼ��ʱ��
	Init_GPIO();//��ʼ��gpio
        
	GPIO_WriteLow(GPIOC, GPIO_PIN_3); //�ط���
	GPIO_WriteHigh(GPIOD, GPIO_PIN_4); //��LED
        
	TIM1_DeInit(); //TIM1��ʼ��
	TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 20000, 0);//20ms��ʱ��,�ڲ�����,����Ƶ,15999
	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
	//�����ж�
	enableInterrupts();	
	
	/* Define FLASH programming time */
	FLASH_DeInit();
	FLASH_Unlock(FLASH_MEMTYPE_DATA); //����eeprom
	FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
	Data_count = FLASH_ReadByte(0x4000); //��ȡң��������
	
	/*�ϵ�ʹ��TIM1���*/
	TIM1_Cmd(ENABLE);
	for(ii = 0; ii < 60000; ii++);
	TIM1_Cmd(DISABLE);
	
	while (1) {
		//ң����ѧϰ��ң����ɾ��ʱ�İ�������
		if (GPIO_ReadInputPin(GPIOC, GPIO_PIN_6) == 0x00) { 
			TIM1_Cmd(ENABLE);
			while (GPIO_ReadInputPin(GPIOC, GPIO_PIN_6) == 0x00) {
				//�������ʱ�䳬��7s
				if (Time_Key_Press > 250) {
					Time_Key_Press = 251;
					TIM1_Cmd(DISABLE);
					ttt = E2PROM_EraseAll();
					Data_count = 0;

					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//����
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteLow(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//����
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//����
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteLow(GPIOD,GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//����
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//����
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteLow(GPIOD, GPIO_PIN_4);
					GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//����
					for(ii = 0; ii < 60000; ii++);
					GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					break;
				} else if (Time_Key_Press > 100) { //�����������4s
					GPIO_WriteLow(GPIOD, GPIO_PIN_4);
				}
			}

			while (GPIO_ReadInputPin(GPIOC, GPIO_PIN_6) == 0x00);
			TIM1_Cmd(DISABLE);
			if(Time_Key_Press > 100 && Time_Key_Press <= 250) {
				//ң����ѧϰ
				ttt = 0;
				while (ttt == 0) {
					ttt = Get1527_Data();
				}
				ttt = E2PROM_WriteOne(ttt);
				Data_count++; //ң������������1

				for(ii = 0; ii < 60000; ii++);
				GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
				GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//����
				for(ii = 0; ii < 60000; ii++);
				GPIO_WriteLow(GPIOD, GPIO_PIN_4);
				GPIO_WriteReverse(GPIOC, GPIO_PIN_3);//����
				for(ii = 0; ii < 60000; ii++);
				GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
				GPIO_WriteHigh(GPIOD, GPIO_PIN_4);//ѧϰ��֮��ָʾ���𣬻ص���̬
			}
			Time_Key_Press=0;
		}
	//ң����ѧϰ��ң����ɾ��ʱ�İ�������--END
	//����ģʽ��ȷ��--END
		do {
			kk = 0;
			ttt = Get1527_Data(); //��ȡ����
			Data_count = FLASH_ReadByte(0x4000); //��ȡң������
			if (ttt != 0 && Data_count != 0) { //�����ȡ���벻Ϊ0, ң��������Ҳ��Ϊ0	
				//�õ�ң������ֵ֮��Ĵ��� 		  			
				for (ii = 0; ii < Data_count; ii++) { //��eeprom�������������
					jj = E2PROM_ReadU32(0x4001 + ii * 3);
					if ((ttt & 0xfffff0) == (jj & 0xfffff0)) { //����������ѧϰ
						kk = 1;
						break;
					}				
				}
					
				if(kk == 1) {
					kk = 0;
					jj = ttt & 0x00000f; //ȡ����λ����λ
					if (jj == 1) { //if KEY1
						GPIO_WriteHigh(GPIOC, GPIO_PIN_7); //���ϼ̵���
                                                GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
					} else if(jj == 2) { //if KEY2
						GPIO_WriteLow(GPIOC, GPIO_PIN_7); //�ͷż̵���
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