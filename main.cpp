#include <windows.h>
#include <iostream>
#include <cctype>


HHOOK mouseHook;
int xButton1Counter = 0;
int xButton2Counter = 0;
char xButton1OutputKey;
char xButton2OutputKey;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && (wParam == WM_XBUTTONDOWN || wParam == WM_XBUTTONUP)) {
        MSLLHOOKSTRUCT* mouseStruct = (MSLLHOOKSTRUCT*)lParam;
        DWORD xButton = GET_XBUTTON_WPARAM(mouseStruct->mouseData);
        
        if (xButton == XBUTTON1) {
            if (wParam == WM_XBUTTONDOWN) {
                xButton1Counter++;
                if (xButton1Counter == 1) {
                    xButton1Counter = 0;
                    INPUT input;
                    input.type = INPUT_KEYBOARD;
                    input.ki.wVk = VkKeyScan(xButton1OutputKey) & 0xFF;
                    input.ki.wScan = 0;
                    input.ki.dwFlags = 0; // 0 for key press
                    input.ki.time = 0;
                    input.ki.dwExtraInfo = 0;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_KEYUP; // for key release
                    SendInput(1, &input, sizeof(INPUT));
                    return 1;
                }
            }
        } else if (xButton == XBUTTON2) {
            if (wParam == WM_XBUTTONDOWN) {
                xButton2Counter++;
                if (xButton2Counter == 1) {
                    xButton2Counter = 0;
                    INPUT input;
                    input.type = INPUT_KEYBOARD;
                    input.ki.wVk = VkKeyScan(xButton2OutputKey) & 0xFF;
                    input.ki.wScan = 0;
                    input.ki.dwFlags = 0; // 0 for key press
                    input.ki.time = 0;
                    input.ki.dwExtraInfo = 0;
                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags = KEYEVENTF_KEYUP; // for key release
                    SendInput(1, &input, sizeof(INPUT));
                    return 1;
                }
            }
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void SetMouseHook() {
    if (!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0))) {
        std::cerr << "Failed to install mouse hook!" << std::endl;
    }
}

void UnhookMouse() {
    UnhookWindowsHookEx(mouseHook);
}

