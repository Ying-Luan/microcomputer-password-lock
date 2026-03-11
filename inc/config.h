/*
 * 配置文件
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

// 12864j液晶显示器
// 后三位分别代表：CS1/2（左右半屏片选，高左低右） RS（指令/数据传送，高数低指） RW（读写，高读低写）
#define WR_COM_AD_L 0x264  // 写左半屏指令地址  100
#define WR_COM_AD_R 0x260  // 写右半屏指令地址  000
#define WR_DATA_AD_L 0x266 // 写左半屏数据地址	110
#define WR_DATA_AD_R 0x262 // 写右半屏数据地址  010
#define RD_BUSY_AD_L 0x265 // 查忙地址          101
#define RD_BUSY_AD_R 0x261 // 查忙地址          001
#define RD_DATA_AD_L 0x267 // 读数据地址        111
#define RD_DATA_AD_R 0x263 // 读数据地址        011
// DB7-DB0初始化设置内容
#define X 0xB8         // 起始显示行基址    10111000，行号为0-7 (10111xxx)
#define Y 0x40         // 起始显示列基址    01000000，列号为0-63(01xxxxxx)
#define FirstLine 0xC0 // 起始显示行        11000000

//  8255

#endif
