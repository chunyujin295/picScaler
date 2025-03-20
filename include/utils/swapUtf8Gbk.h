/*************************************************
 * 描述：字符串编码转换工具 (用于处理中文路径问题)
 *
 * File：swapUtf8Gbk.h
 * Author：yangzhicheng@mozihealthcare.cn
 * Date：2025-01-21
 * Update：
 * ************************************************/
#ifndef SWAPUTF8GBK_H
#define SWAPUTF8GBK_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef PLAT_WIN
// 勿删：windows 头文件引用顺序会导致编译错误
#include <WinSock2.h>
#include <windows.h>

inline std::string gbkToUtf8(const char* src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, nullptr, 0);
	auto* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, nullptr, nullptr);
	std::string strTemp = str;
	delete[] wstr;
	delete[] str;
	return strTemp;
}

inline std::string utf8ToGbk(const char* src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, nullptr, 0);
	auto* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, nullptr, 0, nullptr, nullptr);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, nullptr, nullptr);
	std::string strTemp(szGBK);
	delete[] wszGBK;
	delete[] szGBK;
	return strTemp;
}

#endif

#endif//SWAPUTF8GBK_H
