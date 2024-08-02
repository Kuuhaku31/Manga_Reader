# Manga Reader

[简体中文](README_CN.md) | [English](README_EN.md) | [日本語](README_JP.md)

A manga reader that solves the problem of viewing single-page manga images conveniently across pages. It uses CMake and the EasyX graphics library.

## Installation and Dependencies

1. Install [CMake](https://cmake.org/download/).
2. Install [EasyX graphics library](http://www.easyx.cn/).

## Usage

1. Select a folder.
The program will automatically recognize and add all image files (supporting ".jpg", ".png", ".jpeg", ".bmp", ".gif") in the folder and its subfolders, and package them as volumes by folder.

2. Select the layout mode.
The layout mode will determine the reading direction of the manga. You need to decide whether to read from right to left or vice versa based on the manga layout.

## Keyboard Commands

Currently only supports keyboard operations

| Key       | Function       |
| --------- | -------------- |
| Enter     | Exit           |
| ESC       | Exit           |
| Page Down | Next page      |
| Page Up   | Previous page  |
| →         | Next page      |
| ←         | Previous page  |
| ↑         | Adjust cross-page to the right |
| ↓         | Adjust cross-page to the left  |
| Space     | Undefined      |
| ,         | Rotate counterclockwise |
| .         | Rotate clockwise |
| ;         | Undefined      |
| '         | Undefined      |
| [         | Next volume    |
| ]         | Previous volume|
| \         | Show page number|
| /         | Combine two pages|
| =         | Zoom in        |
| -         | Zoom out       |
| +         | Zoom in        |
| -         | Zoom out       |

Note: When the cross-page does not match, use the "↑" and "↓" keys to adjust.

## Contribution Guide

Welcome to contribute code or report issues. Please follow these steps:

1. Fork this repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -am 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Create a new Pull Request.