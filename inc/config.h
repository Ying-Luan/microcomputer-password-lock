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
#define PA_Addr 0x270  // A口地址270，控制LED
#define PB_Addr 0x271  // B口地址271，键盘列扫描信号
#define PC_Addr 0x272  // C口地址272，读键盘KL1 KL2信号
#define CON_Addr 0x273 // 控制字地址273

// keyboard
#define KEY_CONFIRM 0x0a // #，确认键
#define KEY_MODIFY 0x0b  // 修改键
#define KEY_ADMIN 0x0c   // 管理员键
#define KEY_OPEN 0x0d
#define KEY_SHIFT 0x0e  // shift键
#define KEY_DELETE 0x0f // 删除键

#define u8 unsigned char // 自定义数据类型u8，用于表示按键的键值
#define u16 unsigned int // 自定义数据类型u16，用于表示整形数据

u8 keyMap[16] =
    {0x01, 0x02, 0x03, KEY_MODIFY,
     0x04, 0x05, 0x06, KEY_OPEN,
     0x07, 0x08, 0x09, KEY_ADMIN,
     0x00, KEY_CONFIRM, KEY_DELETE, KEY_SHIFT};

#endif
