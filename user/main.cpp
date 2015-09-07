
#include "ebox.h"
#include "mmc_sd.h"
#include "ff.h"
#include "wrapperdiskio.h"
#include "bootloader.h"
#include "DataFlash.h"

extern void attachSDCardToFat(SD* sd);

USART uart1(USART1,PA9,PA10);
SD sd(PB12,SPI2,PB13,PB14,PB15);

FLASHCLASS flash;

#define FLASH_APP1_ADDR     0x08010000      //???????????(???FLASH)  


extern void attachSDCardToFat(SD* sd);
static FATFS fs;            // Work area (file system object) for logical drive
FIL fsrc;            // �ļ��ṹ
FRESULT res;
u8 ret;

u8 readBuf[1024] ;
u32 br=0;
const char bootpath[]="/ebox_os.bin";
void fileOpt()
{
	/////////////////////////////////////////////////////////////////////////////////////
	u32 readsize;
	u32 buflen;
	buflen = sizeof(readBuf);
	res = f_open(&fsrc,bootpath, FA_READ);//û������ļ��򴴽����ļ�
	if(res==FR_OK)
	{
		uart1.printf("�ҵ������ļ�%s\r\n",bootpath);	
		uart1.printf("���ļ���С��%0.1fk\r\n",fsrc.fsize/1024.0);	
	}
	readsize = 0;
	uart1.printf("�ļ������С�����\r\n");	
	do
	{
		res=f_read(&fsrc,readBuf,buflen,&br);
		if(res==FR_OK)
		{
			// uart1.printfln((const char*)readBuf,sizeof(readBuf));
				flash.write(0x08010000+readsize,readBuf,sizeof(readBuf));

		}
		else				
			{uart1.printf("��ȡ����ʧ�ܣ�\r\n");}
		readsize+=buflen;
		f_lseek(&fsrc,readsize);
	
	}while(br==buflen);
	uart1.printf("�ļ��������\r\n");
  f_close(&fsrc);//�ر��ļ�

}

void setup()
{
	eBoxInit();
	PB8->mode(OUTPUT_PP);
	uart1.begin(9600);
	uart1.printf("===================\r\n");
	uart1.printf("bootloader begin�� \r\n");
	ret = sd.begin();
	if(!ret)
		uart1.printf("\r\nsdcard init ok!\r\n");
	else
		uart1.printf("fail \r\n");
	attachSDCardToFat(&sd);

		PB8->write(!PB8->read());
		delay_ms(100);
		PB8->write(!PB8->read());
		delay_ms(100);
		PB8->write(!PB8->read());
		delay_ms(1000);

	
	f_mount(0, &fs);

		fileOpt();
	uart1.printf("ִ��Ӧ�ó���\r\n");
		delay_ms(100);

  iap_load_app(FLASH_APP1_ADDR);
}
int main(void)
{
	setup();
	while(1)
	{
		
		uart1.printf("runing while\r\n");
		PB8->write(!PB8->read());
		delay_ms(100);

	}


}




