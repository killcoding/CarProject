
#ifndef _D_FLASH_H
#define _D_FLASH_H



extern void  DFlash_Init(void);
extern word  DFlash_Read (word destination);
extern void  DFlash_Write(word ADDR16);
extern void  DFlash_Erase(word ADDR16);

extern word  data_Address;
extern word  Buffer[4];

#endif