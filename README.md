# 微机原理课设 - 密码锁系统

## 使用方法

确保已经安装了 `gcc` 编译器和 `make` 工具，然后在项目根目录下运行以下命令：

```bash
make
```

将生成的 [`output/output.c`](output/output.c) 文件烧录到硬件中即可使用。

## 键位

|     |     |     |        |
| --- | --- | --- | ------ |
| a/1 | b/2 | c/3 | Modify |
| d/4 | e/5 | f/6 | Open   |
| g/7 | h/8 | i/9 | Admin  |
| j/0 | #   | Del | Shift  |

## 功能

* 点击 `Open` 键可以尝试开锁
  * 输入正确密码后会显示`密码正确`，否则重新输入
  * 连续输入三次错误密码后会显示 `系统锁定`，并且黄灯闪烁5s、报警
  * 双击 `shift` 键可以显示真实密码
* 点击 `Modify` 键可以修改密码
  * 输入正确密码后会显示 `请输入新密码`
  * 输入新密码后会显示 `修改成功`
  * 输入密码错误后会显示 `系统锁定`，并且黄灯闪烁5s、报警
* 点击 `Admin` 键可以进入管理员模式
  * 输入正确管理员密码后会显示 `密码正确`，重置用户初始密码为 `123456`
  * 输入错误管理员密码后会显示 `系统锁定`，并且黄灯闪烁5s、报警，然后系统退出
* 点击 `Del` 键可以删除输入的密码
* 点击 `Shift` 键可以输入字母

## 代码结构

| 目录或文件 | 功能 |
| --- | --- |
| [`inc/`](inc/) | 头文件目录 |
| [`inc/config.h`](inc/config.h) | 配置文件 |
| [`inc/font.h`](inc/font.h) | 字库文件 |
| [`inc/hal.h`](inc/hal.h) | 硬件抽象层文件 |
| [`inc/io.h`](inc/io.h) | I/O 驱动文件 |
| [`inc/lcd.h`](inc/lcd.h) | LCD 驱动文件 |
| [`inc/timer.h`](inc/timer.h) | 定时器文件 |
| [`inc/ui.h`](inc/ui.h) | 用户界面文件 |
| [`output/`](output/) | 输出目录 |
| [`output/output.c`](output/output.c) | 输出文件 |
| [`src/`](src/) | 源代码目录 |
| [`src/main.c`](src/main.c) | 主函数文件 |

## 参考

* [4X4矩阵式密码锁（西电微机原理课设第八题）_4*4矩阵式有修改密码功能的密码锁程序ppt-CSDN博客](https://blog.csdn.net/Jack_ccp/article/details/137183288)
