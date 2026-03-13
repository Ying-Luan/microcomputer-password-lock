/*
 * 用户界面相关函数
 */
#ifndef __UI_H__
#define __UI_H__

#include "font.h"
#include "lcd.h"

void show4Characters(u8 *char1, u8 *char2, u8 *char3, u8 *char4)
{
  WordDisL(2, 32, char1); // 第2行,第32列，左半屏，显示一个字子程序
  WordDisL(2, 48, char2);
  WordDisR(2, 0, char3); // 右半屏，显示一个字子程序
  WordDisR(2, 16, char4);
}

// 第2行显示"密码错误"
void showWrongPwd()
{
  show4Characters(Line1_1, Line1_2, Line1_3, Line1_4);
}

// 第2行显示"密码正确"
void showCorrectPwd()
{
  show4Characters(Line1_1, Line1_2, Line1_5, Line1_6);
}

// 第2行显示"系统锁定"
void showSystemLocked()
{
  show4Characters(Line2_1, Line2_2, Line2_3, Line2_4);
}

// 第2行显示"开放输入"
void showEnableInput()
{
  show4Characters(Line3_1, Line3_2, Line3_3, Line3_4);
}

// 第2行显示"修改成功"
void showModifiedSuccess()
{
  show4Characters(Line4_1, Line4_2, Line4_3, Line4_4);
}

// 第2行显示"输入按键"
void showInputButton()
{
  show4Characters(Line6_1, Line6_2, Line6_3, Line6_4);
}

// 第2行显示"输入密码"
void showEnterPwd()
{
  show4Characters(Line3_3, Line3_4, Line1_1, Line1_2);
}

// 第2行显示"再次输入"
void showEnterAgain()
{
  show4Characters(Line7_1, Line7_2, Line3_3, Line3_4);
}

// 显示*
// count表示要显示多少个*
void showStars(u16 count)
{
  u16 i;
  for (i = 0; i < count; i++)
  {
    if (i < 3)
      WordDisL(2, 16 + 16 * i, Line5_1);
    else if (i < 6)
      WordDisR(2, 16 * (i - 3), Line5_1);
    else
      WordDisL(4, 16 + 16 * (i - 6), Line5_1);
  }
}

// 显示"旧的密码"
void showUsedPwd()
{
  show4Characters(Line8_2, Line8_3, Line1_1, Line1_2);
}

// 显示"新的密码"
void showNewPwd()
{
  show4Characters(Line8_1, Line8_3, Line1_1, Line1_2);
}

// 显示"重置成功"
void showResetSuccess()
{
  show4Characters(Line9_1, Line9_2, Line4_3, Line4_4);
}

// 显示"重新输入"
void showReEnter()
{
  show4Characters(Line9_1, Line8_1, Line3_3, Line3_4);
}

#endif
