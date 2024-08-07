# 漫画阅读器

一个翻页漫画阅读器，解决了单页漫画图片无法方便地观看跨页的问题。采用了 CMake 和 EasyX 图形库。

## 安装和依赖

1. 安装 [CMake](https://cmake.org/download/)。
2. 安装 [EasyX 图形库](http://www.easyx.cn/)。

## 使用方法

1. 选择文件夹。
程序将自动识别并添加该文件夹及其子文件夹中的所有图片文件（支持“.jpg”、“.png”、“.jpeg”、“.bmp”、“.gif”），并按文件夹为单位分别封装为卷。

2. 选择排版模式。
排版模式将决定漫画的阅读方向，你需要根据漫画排版来决定是从右向左阅读还是反过来。

注意：路径不能含有汉字、假名等特殊字符，否则会无法正常读取！！！！

## 键盘指令

目前只支持键盘操作

| 键盘按键  | 功能       |
| -------- | ---------- |
| 回车键    | 退出       |
| ESC 键   | 退出       |
| Page Down| 向右翻页   |
| Page Up  | 向左翻页   |
| →键      | 向右翻页   |
| ←键      | 向左翻页    |
| ↑键      | 向右调整跨页   |
| ↓键      | 向左调整跨页   |
| 空格键   | 未定义      |
| , 键     | 逆时针旋转 |
| . 键     | 顺时针旋转 |
| ; 键     | 未定义     |
| ' 键     | 未定义     |
| [ 键     | 下一卷     |
| ] 键     | 上一卷     |
| \ 键     | 显示页码   |
| / 键     | 合并两页   |
| = 键     | 放大       |
| - 键     | 缩小       |
| + 键     | 放大       |
| - 键     | 缩小       |

注：当跨页不匹配时需要利用“↑”、“↓”键来调整。

## 贡献指南

欢迎贡献代码或报告问题。请遵循以下步骤：

1. Fork 本仓库。
2. 创建一个新的分支 (`git checkout -b feature-branch`)。
3. 提交你的更改 (`git commit -am 'Add new feature'`)。
4. 推送到分支 (`git push origin feature-branch`)。
5. 创建一个新的 Pull Request。
