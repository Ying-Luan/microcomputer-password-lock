/*
 * Hardware Abstraction Layer，硬件抽象层
 */
#ifndef __HAL_H__
#define __HAL_H__

// 输出字节到硬件端口中
extern void outportb(unsigned int, char);
// 从指定硬件端口读入一个字节
extern char inportb(unsigned int);

#endif
