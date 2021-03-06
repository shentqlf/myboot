
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
FIL fsrc;            // 文件结构

FRESULT res;
u8 ret;
	
SD sd(PB12,SPI2,PB13,PB14,PB15);

u8 buf[100];
u8 readBuf[6] ;
u32 bw=0;
u32 br=0;

void fileOpt()
{
	for(int i=0;i<100;i++)
		buf[i] = '1';
	res = f_open(&fsrc,"/12345.txt",FA_WRITE | FA_CREATE_ALWAYS| FA_READ);//没有这个文件则创建该文件
	uart1.printf("\r\n");

	if(res==FR_OK)
	{
		uart1.printf("创建文件或打开文件成功  O(∩_∩)O\r\n");
		uart1.printf("该文件属性:%d\r\n",fsrc.flag);
		uart1.printf("该文件大小：%d\r\n",fsrc.fsize);
		uart1.printf("该文件读写开始处：%d\r\n",fsrc.fptr);
		uart1.printf("该文件开始簇号:%d\r\n",fsrc.org_clust);
		uart1.printf("该文件当前簇号：%d\r\n",fsrc.curr_clust);
		uart1.printf("该文件当前扇区号:%d\r\n",fsrc.dsect);

		f_lseek(&fsrc,0);
		do
		{ 
		 res = f_write(&fsrc, buf, sizeof(buf),&bw); 
		if(res) 
			 { 
				 uart1.printf("write error : %d\r\n",res); 
				 break; 
			 }  
		uart1.printf("write ok!\r\n"); 
		}  
			while (bw < sizeof(buf));  //  判断是否写完(bw > 100，表示写入完成)
	}
	else if(res==FR_EXIST)
		uart1.printf("该文件已存在\r\n");
	else
		uart1.printf("创建文件或打开文件失败~~~~(>_<)~~~~ \r\n");
	f_close(&fsrc);//关闭文件
	
	/////////////////////////////////////////////////////////////////////////////////////
	u32 readsize;
	u32 buflen;
	buflen = sizeof(readBuf);
	res = f_open(&fsrc,"/12345.txt", FA_READ);//没有这个文件则创建该文件
	if(res==FR_OK)
	{
		uart1.printf("该文件大小：%d\r\n",fsrc.fsize);	
	}
	readsize = 0;
	do
	{
		res=f_read(&fsrc,readBuf,buflen,&br);
		if(res==FR_OK)
		{
//			 uart1.printf("成功读取数据：%dBytes\r\n",br);
//			 uart1.printf("data:%s\r\n",readBuf);
			 uart1.printfln((const char*)readBuf,sizeof(readBuf));
		}
		else				
			{uart1.printf("读取数据失败！\r\n");}
		readsize+=buflen;
		f_lseek(&fsrc,readsize);
	
	}while(br==buflen);
	uart1.printf("文件读取到末尾！\r\n");
  f_close(&fsrc);//关闭文件


	

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
	fileOpt();
	while(1)
	{
		
	uart1.printf("\r\nrunning！");
		delay_ms(1000);
	}


}




