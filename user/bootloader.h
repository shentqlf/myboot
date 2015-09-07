#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H
#include "stm32f10x.h"
#ifdef __cplusplus
 extern "C" {
#endif 

	 void iap_load_app(u32 appxaddr);

#ifdef __cplusplus
 }
#endif 

#endif
 