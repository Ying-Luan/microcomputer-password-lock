/*
 * 液晶显示器控制程序
 */
#ifndef __LCD_H__
#define __LCD_H__

#include "config.h"
#include "hal.h"

//***************************************
// 基本控制
//***************************************
// 写左半屏控制指令,需要先写指令才能写数据
void WRComL(u8 _data)
{
  outportb(WR_COM_AD_L, _data);
  while (inportb(RD_BUSY_AD_R) & 0x80) // 检查液晶显示是否处于忙状态 BUSY位为DB7, 所以只需要 & 10000000B 来判断是否BUSY，下同
    ;
}

// 写右半屏控制指令,需要先写指令才能写数据
void WRComR(u8 _data)
{
  outportb(WR_COM_AD_R, _data);
  while (inportb(RD_BUSY_AD_R) & 0x80) // 检查液晶显示是否处于忙状态
    ;
}

// 写左半屏数据
void WRDataL(u8 _data)
{
  outportb(WR_DATA_AD_L, _data);
  while (inportb(RD_BUSY_AD_R) & 0x80) // 检查液晶显示是否处于忙状态
    ;
}

// 写右半屏数据
void WRDataR(u8 _data)
{
  outportb(WR_DATA_AD_R, _data);
  while (inportb(RD_BUSY_AD_R) & 0x80) // 检查液晶显示是否处于忙状态
    ;
}

// 显示左半屏数据，count-显示数据个数
void DisplayL(u8 *pt, char count)
{
  while (count--)
  {
    WRDataL(*pt++); // 写左半屏数据
  }
}

// 显示右半屏数据，count-显示数据个数
void DisplayR(u8 *pt, char count)
{
  while (count--)
  {
    WRDataR(*pt++); // 写右半屏数据
  }
}

// 设置左半屏起始显示行列地址,x-X起始行序数(0-7)，y-Y起始列序数(0-63)
void SETXYL(u8 x, u8 y)
{
  WRComL(x + X); // 行地址=行序数+行基址
  WRComL(y + Y); // 列地址=列序数+列基址
}

// 设置右半屏起始显示行列地址,x:X起始行序数(0-7)，y:Y起始列序数(0-63)
void SETXYR(u8 x, u8 y)
{
  WRComR(x + X); // 行地址=行序数+行基址
  WRComR(y + Y); // 列地址=列序数+列基址
}

//***************************************
// 显示图形
//***************************************

// 显示字体，显示一个数据要占用X行两行位置

// 右半屏显示一个字节：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-待显示数据的首地址，显示类型为ASCII码范围，总计16x8
void ByteDisR(u8 x, u8 y, u8 *pt)
{
  SETXYR(x, y);        // 设置起始显示行列地址
  DisplayR(pt, 8);     // 显示上半行数据，8x8
  SETXYR(x + 1, y);    // 设置起始显示行列地址
  DisplayR(pt + 8, 8); // 显示下半行数据，8x8
}

// 右半屏显示一个字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-待显示数据的首地址，显示类型为GBK或Unicode，总计16x16
void WordDisR(u8 x, u8 y, u8 *pt)
{
  SETXYR(x, y);          // 设置起始显示行列地址
  DisplayR(pt, 16);      // 显示上半行数据 8x16bit
  SETXYR(x + 1, y);      // 设置起始显示行列地址
  DisplayR(pt + 16, 16); // 显示下半行数据 8x16bit
}

// 左半屏显示一个字节：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void ByteDisL(u8 x, u8 y, u8 *pt)
{
  SETXYL(x, y);        // 设置起始显示行列地址
  DisplayL(pt, 8);     // 显示上半行数据
  SETXYL(x + 1, y);    // 设置起始显示行列地址
  DisplayL(pt + 8, 8); // 显示下半行数据
}

// 左半屏显示一个字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void WordDisL(u8 x, u8 y, u8 *pt)
{
  SETXYL(x, y);          // 设置起始显示行列地址
  DisplayL(pt, 16);      // 显示上半行数据
  SETXYL(x + 1, y);      // 设置起始显示行列地址
  DisplayL(pt + 16, 16); // 显示下半行数据
}

// 清屏
void LCDClear()
{
  // 清左半屏
  u8 x, y;
  char j;
  x = 0;                  // 起始行，第0行
  y = 0;                  // 起始列，第0列
  for (x = 0; x < 8; x++) // 共8行
  {
    SETXYL(x, y); // 设置起始显示行列地址
    j = 64;
    while (j--)
      WRDataL(0);
  }
  // 清右半屏
  x = 0;                  // 起始行，第0行
  y = 0;                  // 起始列，第0列
  for (x = 0; x < 8; x++) // 共8行
  {
    SETXYR(x, y); // 设置起始显示行列地址
    j = 64;
    while (j--)
      WRDataR(0);
  }
}

// 液晶初始化
void LCD_INIT()
{
  WRComL(0x3e);      // 初始化左半屏，关显示
  WRComL(FirstLine); // 设置起始显示行，第0行
  WRComR(0x3e);      // 初始化右半屏，关显示
  WRComR(FirstLine); // 设置起始显示行，第0行
  LCDClear();        // 清屏
  WRComL(0x3f);      // 开显示
  WRComR(0x3f);      // 开显示
}

#endif
