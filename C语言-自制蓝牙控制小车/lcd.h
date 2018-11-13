#ifndef _LCD_H
#define _LCD_H

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

void    DisableHC595(void);
void    WriteDataLCD(u8 WDLCD);
void    WriteCommandLCD(u8 WCLCD,BuysC);
u8      ReadDataLCD(void);
u8      ReadStatusLCD(void);
void    LCDInit(void);
void    LCDClear(void);
void    LCDFlash(void);
void    DisplayListChar(u8 X, u8 Y, u8 code *DData);
void    DisplayImage (u8 code *DData);
void 	DisplayNum (u8 X, u8 Y, u8 num);

#endif