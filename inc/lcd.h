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
void WRComL(unsigned char _data)
{
  outportb(WR_COM_AD_L, _data);
  while (inportb(RD_BUSY_AD_R) & 0x80) // 检查液晶显示是否处于忙状态 BUSY位为DB7, 所以只需要 & 10000000B 来判断是否BUSY，下同
  {
    ;
  }
}

// 写右半屏控制指令,需要先写指令才能写数据
void WRComR(unsigned char _data)
{
  outportb(WR_COM_AD_R, _data);
  while (inportb(RD_BUSY_AD_R) & 0x80) // 检查液晶显示是否处于忙状态
  {
    ;
  }
}

// 写左半屏数据
void WRDataL(unsigned char _data)
{
  outportb(WR_DATA_AD_L, _data);
  while (inportb(RD_BUSY_AD_R) & 0x80) // 检查液晶显示是否处于忙状态
  {
    ;
  }
}

// 写右半屏数据
void WRDataR(unsigned char _data)
{
  outportb(WR_DATA_AD_R, _data);
  while (inportb(RD_BUSY_AD_R) & 0x80) // 检查液晶显示是否处于忙状态
  {
    ;
  }
}

// 显示左半屏数据，count-显示数据个数
void DisplayL(unsigned char *pt, char count)
{
  while (count--)
  {
    WRDataL(*pt++); // 写左半屏数据
  }
}

// 显示右半屏数据，count-显示数据个数
void DisplayR(unsigned char *pt, char count)
{
  while (count--)
  {
    WRDataR(*pt++); // 写右半屏数据
  }
}

#endif
