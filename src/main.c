#include "config.h"
#include "hal.h"
#include "font.h"
#include "lcd.h"
//***************************************
// 基本控制
//***************************************
// 设置左半屏起始显示行列地址,x-X起始行序数(0-7)，y-Y起始列序数(0-63)
void SETXYL(unsigned char x, unsigned char y)
{
	WRComL(x + X); // 行地址=行序数+行基址
	WRComL(y + Y); // 列地址=列序数+列基址
}

// 设置右半屏起始显示行列地址,x:X起始行序数(0-7)，y:Y起始列序数(0-63)
void SETXYR(unsigned char x, unsigned char y)
{
	WRComR(x + X); // 行地址=行序数+行基址
	WRComR(y + Y); // 列地址=列序数+列基址
}

//***************************************
// 显示图形
//***************************************
// 显示左半屏一行图形,A-X起始行序数(0-7)，B-Y起始列地址序数(0-63)
void LineDisL(unsigned char x, unsigned char y, unsigned char *pt)
{
	SETXYL(x, y);			// 设置起始显示行列
	DisplayL(pt, 64); // 显示数据
}

// 显示右半屏一行图形,A-X起始行地址序数(0-7)，B-Y起始列地址序数(0-63)
void LineDisR(unsigned char x, unsigned char y, unsigned char *pt)
{
	SETXYR(x, y);			// 设置起始显示行列
	DisplayR(pt, 64); // 显示数据
}

// 显示字体，显示一个数据要占用X行两行位置

// 右半屏显示一个字节：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-待显示数据的首地址，显示类型为ASCII码范围，总计16x8
void ByteDisR(unsigned char x, unsigned char y, unsigned char *pt)
{
	SETXYR(x, y);				 // 设置起始显示行列地址
	DisplayR(pt, 8);		 // 显示上半行数据，8x8
	SETXYR(x + 1, y);		 // 设置起始显示行列地址
	DisplayR(pt + 8, 8); // 显示下半行数据，8x8
}

// 右半屏显示一个字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-待显示数据的首地址，显示类型为GBK或Unicode，总计16x16
void WordDisR(unsigned char x, unsigned char y, unsigned char *pt)
{
	SETXYR(x, y);					 // 设置起始显示行列地址
	DisplayR(pt, 16);			 // 显示上半行数据 8x16bit
	SETXYR(x + 1, y);			 // 设置起始显示行列地址
	DisplayR(pt + 16, 16); // 显示下半行数据 8x16bit
}

// 左半屏显示一个字节：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void ByteDisL(unsigned char x, unsigned char y, unsigned char *pt)
{
	SETXYL(x, y);				 // 设置起始显示行列地址
	DisplayL(pt, 8);		 // 显示上半行数据
	SETXYL(x + 1, y);		 // 设置起始显示行列地址
	DisplayL(pt + 8, 8); // 显示下半行数据
}

// 左半屏显示一个字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void WordDisL(unsigned char x, unsigned char y, unsigned char *pt)
{
	SETXYL(x, y);					 // 设置起始显示行列地址
	DisplayL(pt, 16);			 // 显示上半行数据
	SETXYL(x + 1, y);			 // 设置起始显示行列地址
	DisplayL(pt + 16, 16); // 显示下半行数据
}
// 清屏
void LCDClear()
{
	// 清左半屏
	unsigned char x, y;
	char j;
	x = 0;									// 起始行，第0行
	y = 0;									// 起始列，第0列
	for (x = 0; x < 8; x++) // 共8行
	{
		SETXYL(x, y); // 设置起始显示行列地址
		j = 64;
		while (j--)
			WRDataL(0);
	}
	// 清右半屏
	x = 0;									// 起始行，第0行
	y = 0;									// 起始列，第0列
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
	WRComL(0x3e);			 // 初始化左半屏，关显示
	WRComL(FirstLine); // 设置起始显示行，第0行
	WRComR(0x3e);			 // 初始化右半屏，关显示
	WRComR(FirstLine); // 设置起始显示行，第0行
	LCDClear();				 // 清屏
	WRComL(0x3f);			 // 开显示
	WRComR(0x3f);			 // 开显示
}

// 第2行显示"密码错误"
void showWrongPwd()
{
	WordDisL(2, 32, Line1_1); // 第2行,第32列，左半屏，显示一个字子程序
	WordDisL(2, 48, Line1_2);
	WordDisR(2, 0, Line1_3); // 右半屏，显示一个字子程序
	WordDisR(2, 16, Line1_4);
}

// 第2行显示"密码正确"
void showCorrectPwd()
{
	WordDisL(2, 32, Line1_1); // 第2行,第32列，左半屏，显示一个字子程序
	WordDisL(2, 48, Line1_2);
	WordDisR(2, 0, Line1_5); // 右半屏，显示一个字子程序
	WordDisR(2, 16, Line1_6);
}

// 第2行显示"系统锁定"
void showSystemLocked()
{
	WordDisL(2, 32, Line2_1); // 第2行,第32列，左半屏，显示一个字子程序
	WordDisL(2, 48, Line2_2);
	WordDisR(2, 0, Line2_3); // 右半屏，显示一个字子程序
	WordDisR(2, 16, Line2_4);
}

// 第2行显示"开放输入"
void showEnableInput()
{
	WordDisL(2, 32, Line3_1); // 第2行,第32列，左半屏，显示一个字子程序
	WordDisL(2, 48, Line3_2);
	WordDisR(2, 0, Line3_3); // 右半屏，显示一个字子程序
	WordDisR(2, 16, Line3_4);
}

// 第2行显示"修改成功"
void showModifiedSuccess()
{
	WordDisL(2, 32, Line4_1); // 第2行,第32列，左半屏，显示一个字子程序
	WordDisL(2, 48, Line4_2);
	WordDisR(2, 0, Line4_3); // 右半屏，显示一个字子程序
	WordDisR(2, 16, Line4_4);
}

// 第2行显示"输入按键"
void showInputButton()
{
	WordDisL(2, 32, Line6_1); // 第2行,第32列，左半屏，显示一个字子程序
	WordDisL(2, 48, Line6_2);
	WordDisR(2, 0, Line6_3); // 右半屏，显示一个字子程序
	WordDisR(2, 16, Line6_4);
}

// 第2行显示"输入密码"
void showEnterPwd()
{
	WordDisL(2, 32, Line3_3); // 第2行,第32列，左半屏，显示一个字子程序
	WordDisL(2, 48, Line3_4);
	WordDisR(2, 0, Line1_1); // 右半屏，显示一个字子程序
	WordDisR(2, 16, Line1_2);
}

// 第2行显示"再次输入"
void showEnterAgain()
{
	WordDisL(2, 32, Line7_1); // 第2行,第32列，左半屏，显示一个字子程序
	WordDisL(2, 48, Line7_2);
	WordDisR(2, 0, Line3_3); // 右半屏，显示一个字子程序
	WordDisR(2, 16, Line3_4);
}

// 延时程序
void delayTime()
{
	unsigned char i;
	unsigned int j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 0xffff; j++)
		{
			;
		}
	}
}

// 显示一个*
void show1Star()
{
	WordDisL(2, 16, Line5_1);
}

// 显示两个*
void show2Stars()
{
	WordDisL(2, 16, Line5_1);
	WordDisL(2, 32, Line5_1);
}

// 显示三个*
void show3Stars()
{
	WordDisL(2, 16, Line5_1);
	WordDisL(2, 32, Line5_1);
	WordDisL(2, 48, Line5_1);
}

// 显示四个*
void show4Stars()
{
	WordDisL(2, 16, Line5_1);
	WordDisL(2, 32, Line5_1);
	WordDisL(2, 48, Line5_1);
	WordDisR(2, 0, Line5_1);
}

// 显示五个*
void show5Stars()
{
	WordDisL(2, 16, Line5_1);
	WordDisL(2, 32, Line5_1);
	WordDisL(2, 48, Line5_1);
	WordDisR(2, 0, Line5_1);
	WordDisR(2, 16, Line5_1);
}

// 显示六个*
void show6Stars()
{
	WordDisL(2, 16, Line5_1);
	WordDisL(2, 32, Line5_1);
	WordDisL(2, 48, Line5_1);
	WordDisR(2, 0, Line5_1);
	WordDisR(2, 16, Line5_1);
	WordDisR(2, 32, Line5_1);
}

// 显示七个*
void show7Stars()
{
	WordDisL(2, 16, Line5_1);
	WordDisL(2, 32, Line5_1);
	WordDisL(2, 48, Line5_1);
	WordDisR(2, 0, Line5_1);
	WordDisR(2, 16, Line5_1);
	WordDisR(2, 32, Line5_1);
	WordDisL(4, 16, Line5_1);
}

// 显示八个*
void show8Stars()
{
	WordDisL(2, 16, Line5_1);
	WordDisL(2, 32, Line5_1);
	WordDisL(2, 48, Line5_1);
	WordDisR(2, 0, Line5_1);
	WordDisR(2, 16, Line5_1);
	WordDisR(2, 32, Line5_1);
	WordDisL(4, 16, Line5_1);
	WordDisL(4, 32, Line5_1);
}

// 显示"旧的密码"
void showUsedPwd()
{
	WordDisL(2, 32, Line8_2);
	WordDisL(2, 48, Line8_3);
	WordDisR(2, 0, Line1_1);
	WordDisR(2, 16, Line1_2);
}

// 显示"新的密码"
void showNewPwd()
{
	WordDisL(2, 32, Line8_1);
	WordDisL(2, 48, Line8_3);
	WordDisR(2, 0, Line1_1);
	WordDisR(2, 16, Line1_2);
}

// 显示"重置成功"
void showResetSuccess()
{
	WordDisL(2, 32, Line9_1);
	WordDisL(2, 48, Line9_2);
	WordDisR(2, 0, Line4_3);
	WordDisR(2, 16, Line4_4);
}

// 显示"重新输入"
void showReEnter()
{
	WordDisL(2, 32, Line9_1);
	WordDisL(2, 48, Line8_1);
	WordDisR(2, 0, Line3_3);
	WordDisR(2, 16, Line3_4);
}

// #########################################################################################
//  8255
#define PA_Addr 0x270	 // A口地址270，控制LED
#define PB_Addr 0x271	 // B口地址271，键盘列扫描信号
#define PC_Addr 0x272	 // C口地址272，读键盘KL1 KL2信号
#define CON_Addr 0x273 // 控制字地址273

#define u8 unsigned char // 自定义数据类型u8，用于表示按键的键值
#define u16 unsigned int // 自定义数据类型u16，用于表示整形数据

extern char inportb(unsigned int);				// 读8255函数
extern void outportb(unsigned int, char); // 写8255函数
unsigned char *address(u8 buf);
u16 present(u8 *buf, u16 i);

u8 pwd[6] = {'1', '2', '3', '4', '5', '6'}; // 初始密码，定义为字符数组
u16 adminPassword = 12345678;								// 管理员密码，定义为整型
u8 tt;																			// 用于main函数，存放用户按下的键的键值

// 延时子程序
void delay(u16 ms) // 1000
{
	u16 i;
	while (ms--) // 1000
	{
		i = 100;
		do
		{
			;
		} while (--i); // 100
	} // 100000
}

// 判断是否有键被按下
u8 AllKey()
{
	u8 i;													 // i是标志，i=0表示没有键被按下，否则表示有键被按下
	outportb(PB_Addr, 0x0);				 // B口送出信号，同时检测所有按键
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
		i = 0xfe;			 // i是B口送出的列扫描信号，初值为1111 1110，表示从第一列开始扫描
		keyResult = 0; // 初始化键值为0
		do
		{
			outportb(PB_Addr, i);	 // 扫描某列
			j = ~inportb(PC_Addr); // 读取KL2 KL1的值
			if (j & 3)						 // 若当前扫描的列中有键按下(KL2 KL1不为全1)
			{
				bNoKey = 0;				// bNoKey标志置0
				if (j & 2)				// 若次行有键闭合(KL2=0 KL1=1)
					keyResult += 8; // 键值+8
			}
			else // 若当前扫描的列中没有键被按下
			{
				keyResult++;				// 键值+1
				i = ((i << 1) | 1); // 让i左移一位，在下一轮循环时好扫描下一列
			}
		} while (bNoKey && (i != 0xff)); // 当bNoKey为0(已有键被按下)或者扫描完所有列时，退出循环
	}

	if (!bNoKey)
	{
		while (AllKey()) // 判断刚才按下的键是否被释放，若未被释放，则一直陷入循环
		{
		}
	}
	return keyResult;
}

// shift键获取字母
u8 shift()
{
	u8 currChar = key();
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
		outportb(PA_Addr, 0xfd);	// 黄灯1111 1101
		outportb(CON_Addr, 0x0e); // 写控制字0000 1110————C口位操作：PC7复位，蜂鸣器响
		delay(250);
		outportb(PA_Addr, 0xff);	// 黄灯熄灭
		outportb(CON_Addr, 0x0f); // 写控制字0000 1111————C口位操作：PC7置位，蜂鸣器停
		delay(250);
	}
}

// 全熄灭
void blackl()
{
	outportb(PA_Addr, 0xff); // 全灭 1111 1111
}

// -----------------------------------------下面是功能函数-----------------------------------------
// 输入密码并与原密码进行比较，如果正确则返回1，否则返回0
u16 checkPwd()
{
	u16 i;
	u8 buf[6];

	//	u16 inputPassword = 0;
	// 重复扫描6次键盘输入
	for (i = 0; i < 6; i++)
	{
		buf[i] = key();
		if (buf[i] == 0x0f) // 若按下了我们设定的微机键盘del键，则清屏后重新输入密码
		{
			i = -1;				 // 先置i=-1，以使continue之后i=0，可以继续按键盘6次
			LCD_INIT();		 // 液晶初始化
			showReEnter(); // 显示"重新输入"汉字
			delayTime();	 // 延时一段时间
			LCD_INIT();		 // 液晶初始化
			continue;			 // 进入下一循环
		}
		if (buf[i] == 0x0e)
			buf[i] = shift();
		else
			buf[i] += (u8)'0';

		LCD_INIT(); // 清屏，液晶初始化
		if (i == 0)
			show1Star(); // 按下1个键，显示1个*
		else if (i == 1)
			show2Stars(); // 按下2个键，显示2个*
		else if (i == 2)
			show3Stars(); // 按下3个键，显示3个*
		else if (i == 3)
			show4Stars(); // 按下4个键，显示4个*
		else if (i == 4)
			show5Stars(); // 按下5个键，显示5个*
		else if (i == 5)
			show6Stars(); // 按下6个键，显示6个*

		i = present(buf, i);
	}

	while (1)
	{
		// 依位比较输入密码和保存的密码，如果中间有一位不匹配，则返回0，完全匹配说明密码正确
		if (key() == 0x0a)
		{
			for (i = 0; i < 6; i++)
			{
				if (buf[i] != pwd[i])
					return 0;
			}
			return 1;
		}
	}
}
//-------------------------------------------------------修改部分
//-------------------------------------------------------新增函数
unsigned char *address(u8 buf)
{
	unsigned char *mpt;
	switch (buf)
	{
	case 0x30:
		mpt = Line6_5;
		break;
	case 0x31:
		mpt = Line6_6;
		break;
	case 0x32:
		mpt = Line6_7;
		break;
	case 0x33:
		mpt = Line6_8;
		break;
	case 0x34:
		mpt = Line6_9;
		break;
	case 0x35:
		mpt = Line7_3;
		break;
	case 0x36:
		mpt = Line7_4;
		break;
	case 0x37:
		mpt = Line7_5;
		break;
	case 0x38:
		mpt = Line7_6;
		break;
	case 0x39:
		mpt = Line7_7;
		break;
	case 0x61:
		mpt = Line7_8;
		break;
	case 0x62:
		mpt = Line7_9;
		break;
	case 0x63:
		mpt = Line9_4;
		break;
	case 0x64:
		mpt = Line8_4;
		break;
	case 0x65:
		mpt = Line8_5;
		break;
	case 0x66:
		mpt = Line8_6;
		break;
	case 0x67:
		mpt = Line8_7;
		break;
	case 0x68:
		mpt = Line8_8;
		break;
	case 0x69:
		mpt = Line8_9;
		break;
	case 0x6a:
		mpt = Line9_3;
		break;
	}
	return (unsigned char *)mpt;
}
u16 present(u8 *buf, u16 i)
{
	if (buf[i] == 0x3d)
	{
		LCD_INIT();
		if (i == 1)
			ByteDisL(2, 20, address(buf[0]));
		if (i == 2)
		{
			ByteDisL(2, 20, address(buf[0]));
			ByteDisL(2, 36, address(buf[1]));
		}

		if (i == 3)
		{
			ByteDisL(2, 20, address(buf[0]));
			ByteDisL(2, 36, address(buf[1]));
			ByteDisL(2, 52, address(buf[2]));
		}

		if (i == 4)
		{
			ByteDisL(2, 20, address(buf[0]));
			ByteDisL(2, 36, address(buf[1]));
			ByteDisL(2, 52, address(buf[2]));
			ByteDisR(2, 4, address(buf[3]));
		}

		if (i == 5)
		{
			ByteDisL(2, 20, address(buf[0]));
			ByteDisL(2, 36, address(buf[1]));
			ByteDisL(2, 52, address(buf[2]));
			ByteDisR(2, 4, address(buf[3]));
			ByteDisR(2, 20, address(buf[4]));
		}
		if (i == 6)
		{
			ByteDisL(2, 20, address(buf[0]));
			ByteDisL(2, 36, address(buf[1]));
			ByteDisL(2, 52, address(buf[2]));
			ByteDisR(2, 4, address(buf[3]));
			ByteDisR(2, 20, address(buf[4]));
			ByteDisR(2, 36, address(buf[5]));
		}
		if (i == 7)
		{
			ByteDisL(2, 20, address(buf[0]));
			ByteDisL(2, 36, address(buf[1]));
			ByteDisL(2, 52, address(buf[2]));
			ByteDisR(2, 4, address(buf[3]));
			ByteDisR(2, 20, address(buf[4]));
			ByteDisR(2, 36, address(buf[5]));
			ByteDisL(4, 20, address(buf[6]));
		}
		i--;
		delayTime(20000);
		LCD_INIT();

		if (i == 0)
			show1Star(); // 按下1个键，显示1个*
		else if (i == 1)
			show2Stars(); // 按下2个键，显示2个*
		else if (i == 2)
			show3Stars(); // 按下3个键，显示3个*
		else if (i == 3)
			show4Stars(); // 按下4个键，显示4个*
		else if (i == 4)
			show5Stars(); // 按下5个键，显示5个*
		else if (i == 5)
			show6Stars();
	}
	return i;
}
//-------------------------------------------------------新增函数
// 更改密码_输入新密码
void changePwd()
{
	u16 i;
	u8 buf[6];
	// u16 inputPassword = 0;
	//  重复6次读取键盘输入
	for (i = 0; i < 6; i++)
	{
		buf[i] = key();
		if (buf[i] == 0x0f) // 若按下了我们设定的微机键盘del键，则清屏后重新输入密码
		{
			i = -1;				 // 先置i=-1，以使continue之后i=0，可以继续按键盘6次
			LCD_INIT();		 // 液晶初始化
			showReEnter(); // 显示"重新输入"汉字
			delayTime();	 // 延时一段时间
			LCD_INIT();		 // 液晶初始化
			continue;			 // 进入下一循环
		}
		if (buf[i] == 0x0e)	 // 如果按下shift键
			buf[i] = shift();	 // 获取字母型变量，原先0-9的数字键现在变为a-j字母键
		else								 // 如果未按下shift键
			buf[i] += (u8)'0'; // 数字变量要转成unsigned char类型，即转成ASCII码型数字，方便显示
		LCD_INIT();
		switch (i)
		{
		case 0:
			show1Star();
			break;
		case 1:
			show2Stars();
			break;
		case 2:
			show3Stars();
			break;
		case 3:
			show4Stars();
			break;
		case 4:
			show5Stars();
			break;
		case 5:
			show6Stars();
			break;
		}
	}
	while (1)
	{
		if (key() == 0x0a)
		{ // 依位次替换原先密码为新密码
			for (i = 0; i < 6; i++)
				pwd[i] = buf[i];
			break;
		}
	}
}

// 修改密码，如果修改密码成功，返回1，否则返回0
u16 modify()
{
	if (checkPwd()) // 首先让用户重新输入密码并与原密码进行比较，如果正确则返回1，否则返回0
	{								// 原密码输入正确
		LCD_INIT();		// 清屏，液晶初始化
		showNewPwd(); // 显示"新的密码"汉字
		changePwd();	// 让用户输入新密码，实现更改密码
		return 1;
	}
	else
		return 0;
}

// 管理员功能：如果管理员密码输入正确，则将开锁密码重置为123456,并返回1，管理员密码只允许数字出现。
u16 adminOption()
{
	u8 i;
	u16 buf[8];
	while (1)
	{
		u16 inputPassword = 0;	// ！！！！！！！！！初始化！！！！！！！！！
		for (i = 0; i < 8; i++) // 管理员输入密码（扫描8次键盘）
		{
			buf[i] = key();
			if (buf[i] == 0x0f) // 若按下了我们设定的微机键盘del键，则清屏后重新输入密码
			{
				i = -1;				 // 先置i=-1，以使continue之后i=0，可以继续按键盘6次
				LCD_INIT();		 // 液晶初始化
				showReEnter(); // 显示"重新输入"汉字
				delayTime();	 // 延时一段时间
				LCD_INIT();		 // 液晶初始化
				continue;			 // 进入下一循环
			}
			LCD_INIT();
			switch (i)
			{
			case 0:
				show1Star();
				break;
			case 1:
				show2Stars();
				break;
			case 2:
				show3Stars();
				break;
			case 3:
				show4Stars();
				break;
			case 4:
				show5Stars();
				break;
			case 5:
				show6Stars();
				break;
			case 6:
				show7Stars();
				break;
			case 7:
				show8Stars();
				break;
			}
		}

		for (i = 0; i < 8; i++) // 计算管理员密码对应的十进制值
		{
			inputPassword = inputPassword * 10 + buf[i];
		}
		while (1)
		{
			if (key() == 0x0a)
				break;
		}
		if (inputPassword == adminPassword)
		{
			// password = 123456; // 重置密码为123456
			for (i = 0; i < 6; i++)
			{
				pwd[i] = i + (u8)'1';
			}
			LCD_INIT();				// 液晶初始化
			showCorrectPwd(); // 显示密码正确
			delayTime();
			return 1;
		}
		// 密码错误，锁定系统
		else
		{
			LCD_INIT();
			delayTime();
			showSystemLocked(); // 第2行显示"系统锁定"
			yellowl();					// 黄灯闪烁5s
			LCD_INIT();					// 液晶初始化
			delayTime();
			return 0; // 进入下一循环
		}
	}
}

// a键代表开锁,如果开锁成功则返回1,如果密码错误超过三次则返回0
u16 openLock()
{
	u16 errorcount = 0;
	u16 t;
	while (1)
	{
		// 若输错3次，返回0
		if (errorcount >= 3)
			return 0;
		// 若输错没到3次，液晶显示提示信息"再次输入"
		if (errorcount != 0 && errorcount != 3)
		{
			LCD_INIT();
			delayTime();
			showEnterAgain(); // 第2行显示"再次输入"
		}

		t = checkPwd(); // 输入密码并与原密码进行比较，如果正确则返回1，否则返回0
		if (t)
			return 1;
		else
			errorcount++; // 输入错误，error数加一
	}
}

void main()
{
	outportb(CON_Addr, 0x81); // 写控制字1000 0001————A口、B口方式0，输出；C口低4位：输入
	outportb(CON_Addr, 0x0f); // 写控制字0000 1111————C口位操作：将PC7置位，控制蜂鸣器
	blackl();									// 一加电应该保持8个LED灯全灭
	while (1)
	{
		// 液晶显示 请输入按键
		LCD_INIT();
		delayTime();
		showInputButton();
		tt = key();
		// 输入a，尝试开锁
		if (tt == 0x0a)
		{
			LCD_INIT();
			delayTime();
			showEnterPwd(); // 第2行显示"输入密码"

			// 如果开锁成功则返回1,如果密码错误超过三次则返回0
			if (openLock())
			{
				LCD_INIT(); // 液晶初始化
				delayTime();
				showCorrectPwd(); // 第2行显示"密码正确"
				delayTime();
				greenl(); // 绿灯亮10s
				redl();		// 红灯亮5s
				blackl(); // 全熄灭
				continue; // 进入下一循环
			}

			// 三次密码错误，锁定系统
			else
			{
				LCD_INIT();
				delayTime();
				showSystemLocked(); // 第2行显示"系统锁定"
				yellowl();					// 黄灯闪烁5s
				continue;						// 进入下一循环
			}
		}

		// 输入b，修改密码功能
		else if (tt == 0x0b)
		{
			// 打印输入原密码
			LCD_INIT();
			showUsedPwd();
			if (modify()) // 修改密码，如果修改密码成功，返回1，否则返回0
			{
				LCD_INIT();
				showModifiedSuccess();
				delayTime();
			}
			// 密码错误，锁定系统
			else
			{
				LCD_INIT();
				delayTime();
				showSystemLocked(); // 第2行显示"系统锁定"
				yellowl();					// 黄灯闪烁5s
				continue;						// 进入下一循环
			}
		}

		// 输入c，进入管理员功能：将开锁密码重置为123456
		else if (tt == 0x0c)
		{
			LCD_INIT();
			showEnterPwd(); // 第2行显示"输入密码"
			delayTime();
			if (adminOption()) // adminOption()函数：如果管理员密码输入正确，将开锁密码重置为123456并返回1
			{
				LCD_INIT();
				showResetSuccess(); // 显示"重置成功"
				delayTime();
			}
		}
	}
}
