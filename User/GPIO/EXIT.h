#ifndef __EXIT_H
#define __EXIT_H	 

#include "sys.h"



void EXTIX_Init(void);//外部中断初始化	

void EXTIX_Disable(u8 extix);
void EXTIX_Enable(u8 extix);

#endif

