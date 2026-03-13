#include "config.h"
#include "hal.h"
#include "io.h"
#include "lcd.h"
#include "timer.h"
#include "ui.h"

u8 pwd[6] = {'1', '2', '3', '4', '5', '6'};								 // 初始密码，定义为字符数组
u8 adminPwd[8] = {'1', '2', '3', '4', '5', '6', '7', '8'}; // 管理员密码，定义为字符数组

// -----------------------------------------底层工具函数-----------------------------------------
// 将键盘输入的字符转换为对应的字模数据指针，ASCII -> 字模数据地址
u8 *address(u8 buf)
{
	u8 *mpt;
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
	return (u8 *)mpt;
}

// -----------------------------------------中层处理函数-----------------------------------------
u16 present(u8 *buf, u16 i)
{
	u16 j;
	LCD_INIT();

	for (j = 0; j < i; j++)
	{
		if (j < 3)
			ByteDisL(2, 20 + 16 * j, address(buf[j]));
		else if (j < 6)
			ByteDisR(2, 4 + 16 * (j - 3), address(buf[j]));
		else
			ByteDisL(4, 20 + 16 * (j - 6), address(buf[j]));
	}

	i--;
	delay(10);
	LCD_INIT();

	showStars(i + 1);

	return i;
}

// -----------------------------------------业务逻辑函数-----------------------------------------
// 输入密码并与原密码进行比较，如果正确则返回1，否则返回0
u16 checkPwd()
{
	u16 i;
	u8 buf[6]; // 只存储 ascii

	// 重复扫描6次键盘输入
	for (i = 0; i < 6; i++)
	{
		buf[i] = key();
		if (buf[i] == KEY_DELETE) // 若按下了我们设定的微机键盘del键，则清屏后重新输入密码
		{
			i = -1;				 // 先置i=-1，以使continue之后i=0，可以继续按键盘6次
			LCD_INIT();		 // 液晶初始化
			showReEnter(); // 显示"重新输入"汉字
			delayTime();	 // 延时一段时间
			LCD_INIT();		 // 液晶初始化
			continue;			 // 进入下一循环
		}
		if (buf[i] == KEY_SHIFT)
		{
			u8 shiftResult = shift();
			if (shiftResult == 0)
				i = present(buf, i);
			else
				buf[i] = shiftResult;
		}
		else
			buf[i] += (u8)'0';

		LCD_INIT();				// 清屏，液晶初始化
		showStars(i + 1); // 显示对应数量的*
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

// 更改密码_输入新密码
void changePwd()
{
	u16 i;
	u8 buf[6];
	//  重复6次读取键盘输入
	for (i = 0; i < 6; i++)
	{
		buf[i] = key();
		if (buf[i] == KEY_DELETE) // 若按下了我们设定的微机键盘del键，则清屏后重新输入密码
		{
			i = -1;				 // 先置i=-1，以使continue之后i=0，可以继续按键盘6次
			LCD_INIT();		 // 液晶初始化
			showReEnter(); // 显示"重新输入"汉字
			delayTime();	 // 延时一段时间
			LCD_INIT();		 // 液晶初始化
			continue;			 // 进入下一循环
		}
		if (buf[i] == KEY_SHIFT) // 如果按下shift键
			buf[i] = shift();			 // 获取字母型变量，原先0-9的数字键现在变为a-j字母键
		else										 // 如果未按下shift键
			buf[i] += (u8)'0';		 // 数字变量要转成u8类型，即转成ASCII码型数字，方便显示
		LCD_INIT();
		showStars(i + 1);
	}
	while (1)
	{
		if (key() == KEY_CONFIRM)
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
	u8 buf[8];
	while (1)
	{
		for (i = 0; i < 8; i++) // 管理员输入密码（扫描8次键盘）
		{
			buf[i] = key();
			if (buf[i] == KEY_DELETE) // 若按下了我们设定的微机键盘del键，则清屏后重新输入密码
			{
				i = -1;				 // 先置i=-1，以使continue之后i=0，可以继续按键盘6次
				LCD_INIT();		 // 液晶初始化
				showReEnter(); // 显示"重新输入"汉字
				delayTime();	 // 延时一段时间
				LCD_INIT();		 // 液晶初始化
				continue;			 // 进入下一循环
			}
			LCD_INIT();
			showStars(i + 1);
		}

		while (1)
		{
			if (key() == KEY_CONFIRM)
				break;
		}
		for (i = 0; i < 8; i++)
			buf[i] += (u8)'0';
		for (i = 0; i < 8; i++)
		{
			if (buf[i] != adminPwd[i])
			{
				// 密码错误处理
				LCD_INIT();
				delayTime();
				showSystemLocked(); // 第2行显示"系统锁定"
				yellowl();					// 黄灯闪烁5s
				LCD_INIT();					// 液晶初始化
				delayTime();
				return 0; // 进入下一循环
			}
		}
		// 密码正确
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

// -----------------------------------------主函数-----------------------------------------
void main()
{
	outportb(CON_Addr, 0x81); // 写控制字1000 0001————A口、B口方式0，输出；C口低4位：输入
	outportb(CON_Addr, 0x0f); // 写控制字0000 1111————C口位操作：将PC7置位，控制蜂鸣器
	blackl();									// 一加电应该保持8个LED灯全灭

	u8 tt; // 存放用户按下的键的键值
	while (1)
	{
		// 液晶显示 请输入按键
		LCD_INIT();
		delayTime();
		showInputButton();
		tt = key();
		// 输入Open，尝试开锁
		if (tt == KEY_OPEN)
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
		else if (tt == KEY_MODIFY)
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
		else if (tt == KEY_ADMIN)
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
