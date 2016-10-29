/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __E2PEOM_H
#define __E2PROM_H

//#include "stm8s_type.h"
#include <stdio.h>
#include <string.h>

u32 E2PROM_ReadU32(unsigned int add);
unsigned char E2PROM_WriteU32(unsigned int add,u32 dat);
unsigned char E2PROM_EraseAll(void);
unsigned char E2PROM_WriteOne(u32 data);
#endif /* E2PROM */