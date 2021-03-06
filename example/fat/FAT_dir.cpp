
#include "ebox.h"
#include "w5500.h"
#include "mmc_sd.h"
#include "ff.h"
#include "wrapperdiskio.h"
#include "udp.h"
extern void attachSDCardToFat(SD* sd);

USART uart1(USART1,PA9,PA10);

static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
DIR DirObject;       //目录结构
FRESULT res;
u8 ret;
	
SD sd(PB12,SPI2,PB13,PB14,PB15);

void dirOpt()
{
	res=f_mkdir("/123");//新建目录只能一级一级的建，即调用一次f_mkdir(),建一层目录而已，目录名不能以数字开头
	if(res==FR_OK)
		uart1.printf("\r\n创建目录成功 !");
	else if(res==FR_EXIST)
		uart1.printf("\r\n所建目录已存在 !");    
	else
		uart1.printf("\r\n创建目录失败~~~~(>_<)~~~~");

	res=f_opendir(&DirObject,"/123");//打开目录
	if(res==FR_OK)
	{
		uart1.printf("\r\n打开目录成功 !");
		uart1.printf("\r\n该目录所在簇号：%d",DirObject.clust);
		uart1.printf("\r\n该目录所在扇区号：%d",DirObject.sect);
	}
	else if(res==FR_NO_PATH)
		uart1.printf("\r\n所找目录路径不存在");    
	else
		uart1.printf("\r\n打开目录失败~~~~(>_<)~~~~");
}
void setup()
{
	eBoxInit();
	uart1.begin(9600);
	ret = sd.begin();
	if(!ret)
		uart1.printf("\r\nsdcard init ok!");
	attachSDCardToFat(&sd);
	
	f_mount(0, &fs);
   
}
u32 count;
int main(void)
{
	setup();
	dirOpt();
	while(1)
	{
		
		delay_ms(1000);
	}


}




