/**************************************************************
时间：2014年10月30日

说明：此文件为xs256 D_Flash 读写程序的底层。用于初次电感值采集
      写入EEPROM
      
**************************************************************/      



#include "Include_Def.h"  

#define READword(address)     ((unsigned int)(*(volatile unsigned int *__near)(address)))

#define DFLASH_LOWEST_START_PAGE        0x00        //定义data flash的起始页
#define DFLASH_START                    0x00100000  //定义data flash的起始地址
#define DFLASH_PAGE_SIZE                0x0400      //定义data flash的大小为1K.
#define DFLASH_PAGE_WINDOW_START        0x0800      //定义data flash页面窗口的起始地址
#define LEDCPU PORTB_PB0
#define LEDCPU_dir DDRB_DDRB0
  
word  data_Address = 0x1000;
//word  date_read_his[4];
word  Buffer[4];//数据缓存区，只能一次写入四个数据   
  
/*********************************************************** 
** 名      称：void DFlash_Init(void) 
** 功      能：DFlash初始化  
** 入口参数：无 
** 出口参数：无 
** 使用说明：无 
************************************************************/  
void DFlash_Init(void)  
{  
    while(FSTAT_CCIF==0);            //等待正在处理的FLASH操作完成  
    FCLKDIV=0x0F;                    //外部晶振为16M.FLASH时钟不超过1M，具体参照手册  
    FCNFG=0x00;                      //禁止中断  
    while(FCLKDIV_FDIVLD==0);        //等待时钟设置成功  
}  
  
/*********************************************************** 
** 名      称：void DFlash_Write(word ADDR16) 
** 功      能：向DFLASH写入数据  
** 入口参数：无 
** 出口参数：无 
** 使用说明：无 
************************************************************/  
void DFlash_Write(word ADDR16)  
{  
    while(FSTAT_CCIF==0);   
    if(FSTAT_ACCERR)           //判断并清除标志位；  
        FSTAT_ACCERR=1;  
    if(FSTAT_FPVIOL)           //判断并清除标志位；  
        FSTAT_FPVIOL=1;  
    FCCOBIX_CCOBIX=0x00;   
    FCCOB=0x1110;         //写入命令和高位地址  
    FCCOBIX_CCOBIX=0x01;  //地址后16位  
    FCCOB=ADDR16;         //写入低16位地址  
    FCCOBIX_CCOBIX=0x02;  //写入第一个数据  
    FCCOB=Buffer[0];  
    FCCOBIX_CCOBIX=0x03;  //写入第二个数据  
    FCCOB=Buffer[1];  
    FCCOBIX_CCOBIX=0x04;  //写入第三个数据  
    FCCOB=Buffer[2];  
    FCCOBIX_CCOBIX=0x05;  //写入第四个数据  
    FCCOB=Buffer[3];    
        
    FSTAT_CCIF=1;         //写入执行命令  
    while(FSTAT_CCIF==0); //等待执行完毕  
}  
  
/*********************************************************** 
** 名      称：word DFlash_Read (word destination) 
** 功      能：由DFLASH读取数据 
** 入口参数：无 
** 出口参数：无 
** 使用说明：无 
************************************************************/  
word DFlash_Read (word destination)  
{  
    byte   lastepage;          //用于存储EPAGE的值  
    byte   epage;              //用于计算EPAGE的值  
    word data;         //读取出的数据  
  
    lastepage = EPAGE;   //保存EPAGE的值  
  
    epage = (byte)((DFLASH_LOWEST_START_PAGE)+(destination >>10));   //计算EPAGE  
    EPAGE=epage;                                                     //给EPAGE赋值  
   
    data = READword((destination & (DFLASH_PAGE_SIZE - 1)) + DFLASH_PAGE_WINDOW_START);  //读取页面窗口中的数据  
  
    EPAGE= lastepage;       //恢复EPAGE的值  
  
    return(data);  
}  
  
/*********************************************************** 
** 名      称：void DFlash_Erase(word ADDR16) 
** 功      能：擦除DFLASH的一个分区 
** 入口参数：无 
** 出口参数：无 
** 使用说明：无 
************************************************************/  
void DFlash_Erase(word ADDR16)  
{     
  while(FSTAT_CCIF==0);  
  if(FSTAT_ACCERR)           //判断并清除标志位；  
      FSTAT_ACCERR=1;  
  if(FSTAT_FPVIOL)           //判断并清除标志位；  
      FSTAT_FPVIOL=1;  
    
  FCCOBIX_CCOBIX=0x00;  
  FCCOB=0x1210;           //写入擦除命令和高位地址  
  FCCOBIX_CCOBIX=0x01;     
  FCCOB=ADDR16;           //写入低16位的地址  
  FSTAT_CCIF=1;           //启动执行命令  
  while(FSTAT_CCIF==0);   //等待执行完成  
}
