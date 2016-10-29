/* Includes ------------------------------------------------------------------*/
//#include "stm8s_lib.h"
#include "stm8s.h"
#include "e2prom.h"


//功能：E2PROM指定起始地址开始读取两个字节的数据并返回该值
//传递参数：ADD，E2PROM读操作的起始高8位地址，ADD+1，低8位地址
//返回值为指定E2PROM地址中的数据。
u32 E2PROM_ReadU32(unsigned int add)
{
   /* Read a byte at a specified address */
    u32 val = 0;
    //u32 b = 0;
    //b = FLASH_ReadByte(add + 1) ;
    val = FLASH_ReadByte(add) + 1;
    //val = ((int)(FLASH_ReadByte(add + 1)))<<8;
    //val = FLASH_ReadByte(add + 2);
    val = val*65536+ FLASH_ReadByte(add+1)*256+FLASH_ReadByte(add+2);
    return val;
}

//功能：E2PROM指定起始地址开始写两个字节的数据并返回成功与否，0失败 1成功
//传递参数：ADD，E2PROM写操作的起始高8位地址，ADD+1，低8位地址
//dat,写入E2PROM指定地址的16位数据
unsigned char E2PROM_WriteU32(unsigned int add,u32 dat)
{
   /* Unlock Data memory */
	 unsigned char val = 0,val2 = 0;
	  val=dat>>16;
	 
	  FLASH_ProgramByte(add , val);	
    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);		
    val2 = FLASH_ReadByte(add);
    if (val != val2)
    {
        /* Error */
        return 0;
    }

		val=(u8)(dat>>8);
    FLASH_ProgramByte(add+1, val);
		FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
		val2 = FLASH_ReadByte(add+1);
    if (val != val2)
    {
        return 0;
    }
		
		val=(u8)dat;
    FLASH_ProgramByte(add+2, val);
		FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
		val2 = FLASH_ReadByte(add+2);
    if (val != val2)
    {
        return 0;
    }

		return 1;
}

//擦除所有E2PROM中的遥控脉冲数据。
//返回1成功，返回0失败
unsigned char E2PROM_EraseAll(void)
{
	/* Erase byte at a specified address & address + 1 */
	unsigned char i,x,val2;
	
  x=FLASH_ReadByte(0x4000);
	if(x==0)return 1;
	
	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	 
	FLASH_ProgramByte(0x4000 , 0);	
  FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);	
  val2 = FLASH_ReadByte(0x4000);
    if (val2 != 0)
    {
        /* Error */
        return 0;
    }
	
	for(i=0;i<x*3;i++)
  {
		 FLASH_ProgramByte(0x4001+i,0);
  }
	return 1;
}

unsigned char E2PROM_WriteOne(u32 data)
{
	unsigned char i,x,val2;
	
        x = FLASH_ReadByte(0x4000);
	x++;
	if(x>200)return 0;//数据太多
	
	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	 
	FLASH_ProgramByte(0x4000 , x);
	FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
      val2 = FLASH_ReadByte(0x4000);
	
    if (val2 != x)
    {
        /* Error */
        return 0;
    }
	
	E2PROM_WriteU32(0x4001+(x-1)*3,data);
	return 1;
}
