#include "bootloader.h"

typedef void (*iapfun)(void); //???????????

iapfun jump2app;

__asm void MSR_MSP(u32 addr) //??????
{
			MSR MSP, r0
			BX r14
}

void iap_load_app(u32 appxaddr)
{

	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)//??????????.

	{ 

		jump2app=(iapfun)*(vu32*)(appxaddr+4);//????????????????(????)

		MSR_MSP(*(vu32*)appxaddr);//???APP????(??????????????????)

		jump2app();//???APP.

}

}
