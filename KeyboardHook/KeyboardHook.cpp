// KeyboardHook.cpp : 定义 DLL 的导出函数。
//
#include "pch.h"
#include "KeyboardHook.h"

using std::string;
static HHOOK g_hKeyboardHook = NULL;
static std::string g_recordedDigits="";
static std::string card_id="";
static BOOLEAN isHook = false;
static BOOLEAN isUnHook = false;
static std::map<std::string, int> cardMap;

// 这是导出变量的一个示例
KEYBOARDHOOK_API int nKeyboardHook=0;

// 这是导出函数的一个示例。
KEYBOARDHOOK_API int fnKeyboardHook(void)
{
    return 0;
}

// 这是已导出类的构造函数。
CKeyboardHook::CKeyboardHook()
{
    return;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

		PKBDLLHOOKSTRUCT pKeyBoard = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);
		if (nCode >= 0 && wParam == WM_KEYDOWN) {
			if (pKeyBoard->vkCode >= '0' && pKeyBoard->vkCode <= '9') {
				g_recordedDigits += static_cast<char>(pKeyBoard->vkCode);
				if (g_recordedDigits.length() == 10) {
					if (cardMap.find(g_recordedDigits) != cardMap.end()) {
						cardMap[g_recordedDigits]++;
					}
					else {
						cardMap[g_recordedDigits] = 1;
					}
					//PostQuitMessage(0);
					//return 0;
					//CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
				}if (g_recordedDigits.length() > 10) {
					g_recordedDigits.clear();
				}
			}
			else {
				g_recordedDigits.clear();
			}
		}
		if (isUnHook) {
			PostQuitMessage(0);
		}
	return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}


void unHook() {
	isUnHook = true;
}
int uninstallHook() {
	int stat = 0;
	if (g_hKeyboardHook != NULL) {
		BOOLEAN s = UnhookWindowsHookEx(g_hKeyboardHook);
		stat = s ? 0 : 1;
		g_hKeyboardHook = NULL;
	}
	isHook = false;
	return stat;
}

const char* getCard() {
	string card = "";
	int cardCount = 0;
	for (const auto& pair : cardMap) {
		if (pair.second > cardCount) {
			cardCount = pair.second;
			card = pair.first;
		}
		std::cout << "-------cont------>CardCount:" << pair.second << "Card:" << pair.first << std::endl;
	}
	std::cout << "-------final------>Card:" << card << std::endl;
	card_id.assign(card);
	return card_id.c_str();
}
void setOutTime(int time) {

}
int installHook() {
	if (isHook) {
		return 1;
	}
	isHook = true;
	isUnHook = false;
	g_recordedDigits.clear();
	card_id.clear();
	cardMap.clear();

	g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
	std::cout << "-------hookStat------>" << (g_hKeyboardHook != NULL ? 0 : 1) << std::endl;
	if (g_hKeyboardHook == NULL) {
		return 1;
	}

	// 进入消息循环，等待按键事件
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	int unHookStat = uninstallHook();
	std::cout << "-------unHookStat------>" << unHookStat << std::endl;
	return 0;
}

