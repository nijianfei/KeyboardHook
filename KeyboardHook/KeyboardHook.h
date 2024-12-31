﻿// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 KEYBOARDHOOK_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// KEYBOARDHOOK_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef KEYBOARDHOOK_EXPORTS
#define KEYBOARDHOOK_API __declspec(dllexport)
#else
#define KEYBOARDHOOK_API __declspec(dllimport)
#endif

#include "framework.h"
#include <string>
#include <map>
#include <iostream>
// 此类是从 dll 导出的
class KEYBOARDHOOK_API CKeyboardHook {
public:
	CKeyboardHook(void);
	// TODO: 在此处添加方法。
};

extern KEYBOARDHOOK_API int nKeyboardHook;

KEYBOARDHOOK_API int fnKeyboardHook(void);
extern "C" {
	KEYBOARDHOOK_API void setOutTime(int);
	KEYBOARDHOOK_API int installHook();
	KEYBOARDHOOK_API void unHook();
	KEYBOARDHOOK_API const char* getCard();
}