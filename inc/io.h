/*
 * 输入输出
 */
#ifndef __IO_H__
#define __IO_H__

#include "config.h"
#include "hal.h"
#include "timer.h"

// 判断是否有键被按下
u8 AllKey()
{
  u8 i;                          // i是标志，i=0表示没有键被按下，否则表示有键被按下
  outportb(PB_Addr, 0x0);        // B口送出信号，同时检测所有按键
  i = (~inportb(PC_Addr) & 0x3); // 读C口数据，若PC1(KL2) PC0(KL1)为1 1，说明没有键被按下，返回0，一旦有某个键被按下，则返回非0
  return i;
}

// 读取键盘输入
u8 key()
{
  u8 i, j, keyResult;
  u8 bNoKey = 1; // 初始化没有键被按下为1
  while (bNoKey)
  {
    if (AllKey() == 0) // 判断是否有键被按下
    {
      continue; // 若当前未按下任何键，则直接进入下一循环，重复监听
    }
    // 下面处理已经有键被按下的情况
    i = 0xfe;      // i是B口送出的列扫描信号，初值为1111 1110，表示从第一列开始扫描
    keyResult = 0; // 初始化键值为0
    do
    {
      outportb(PB_Addr, i);  // 扫描某列
      j = ~inportb(PC_Addr); // 读取KL2 KL1的值
      if (j & 3)             // 若当前扫描的列中有键按下(KL2 KL1不为全1)
      {
        bNoKey = 0;       // bNoKey标志置0
        if (j & 2)        // 若次行有键闭合(KL2=0 KL1=1)
          keyResult += 8; // 键值+8
      }
      else // 若当前扫描的列中没有键被按下
      {
        keyResult++;        // 键值+1
        i = ((i << 1) | 1); // 让i左移一位，在下一轮循环时好扫描下一列
      }
    } while (bNoKey && (i != 0xff)); // 当bNoKey为0(已有键被按下)或者扫描完所有列时，退出循环
  }

  if (!bNoKey)
  {
    while (AllKey()) // 判断刚才按下的键是否被释放，若未被释放，则一直陷入循环
      ;
  }
  return keyMap[keyResult];
}

// shift键获取字母
u8 shift()
{
  u8 currChar = key();
  if (currChar == KEY_SHIFT)
    return 0;
  while (currChar > 0x09) // 若按下的键值大于9，则不断要求用户重新输入
    currChar = key();
  return currChar + (u8)'a'; // 在'a'的ASCII码基础上进行0-9的偏移，得到不同字母('a'-'j')的ASCII码
}

// 绿灯亮10s
void greenl()
{
  outportb(PA_Addr, 0xfe); // 绿灯1111 1110（参考硬件F4模块电路原理图）
  delay(10000);
}

// 红灯亮5s
void redl()
{
  outportb(PA_Addr, 0xfb); // 红灯1111 1011
  delay(5000);
}

// 黄灯闪烁5s及蜂鸣器发出警报声
void yellowl()
{
  u8 i;
  for (i = 0; i < 10; i++)
  {
    outportb(PA_Addr, 0xfd);  // 黄灯1111 1101
    outportb(CON_Addr, 0x0e); // 写控制字0000 1110————C口位操作：PC7复位，蜂鸣器响
    delay(250);
    outportb(PA_Addr, 0xff);  // 黄灯熄灭
    outportb(CON_Addr, 0x0f); // 写控制字0000 1111————C口位操作：PC7置位，蜂鸣器停
    delay(250);
  }
}

// 全熄灭
void blackl()
{
  outportb(PA_Addr, 0xff); // 全灭 1111 1111
}

#endif
