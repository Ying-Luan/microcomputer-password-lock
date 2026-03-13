/*
 * 定时器相关函数
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include "config.h"

// 延时程序
void delayTime()
{
  u8 i;
  u16 j;
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 0xffff; j++)
      ;
  }
}

// 延时子程序
void delay(u16 ms) // 1000
{
  u16 i, j;
  while (ms--) // 1000
  {
    for (j = 0; j < 100; j++)
    {
      i = 1000;
      while (i--)
        ;
    }
  } // 100000
}

#endif
