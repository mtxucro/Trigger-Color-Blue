#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <iomanip>

using namespace std;

bool isKeyPresset(int vKey) {
    return GetAsyncKeyState(vKey) < 0;
}

COLORREF GetPixelColorFromCenterWithOffset(int xOffset, int yOffset, HDC hdc)
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int centerX = screenWidth / 2 + xOffset;
    int centerY = screenHeight / 2 + yOffset;

    HDC hdcMem = CreateCompatibleDC(hdc);
    if (!hdcMem) {
        return RGB(0, 0, 0);
    }

    HBITMAP hBitMap = CreateCompatibleBitmap(hdc, 1, 1);
    if (!hBitMap) {
        DeleteDC(hdcMem);
        return RGB(0, 0, 0);
    }

    SelectObject(hdcMem, hBitMap);
    BitBlt(hdcMem, 0, 0, 1, 1, hdc, centerX, centerY, SRCCOPY);

    COLORREF color = GetPixel(hdc, centerX, centerY);

    DeleteDC(hdcMem);
    DeleteObject(hBitMap);

    return color;
}

void SchemaMouseEvent(int ms) {
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.time = 0;

    this_thread::sleep_for(chrono::milliseconds(ms));

    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(input));

    this_thread::sleep_for(chrono::milliseconds(1));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(input));
}

int main()
{
    setlocale(LC_ALL, "");

    int threshold = 20;
    int ms = 20;
    int bindkey = VK_XBUTTON1;

    while (true) {

        if (isKeyPresset(bindkey)) {
            HDC hdc = GetDC(NULL);
            if (hdc == NULL) {
                return 1;
            }

            COLORREF color1 = GetPixelColorFromCenterWithOffset(2, 2, hdc);
            this_thread::sleep_for(chrono::milliseconds(11));
            COLORREF color2 = GetPixelColorFromCenterWithOffset(2, 2, hdc);

            BYTE r1 = GetRValue(color1);
            BYTE g1 = GetGValue(color1);
            BYTE b1 = GetBValue(color1);

            BYTE r2 = GetRValue(color2);
            BYTE g2 = GetGValue(color2);
            BYTE b2 = GetBValue(color2);

            if (abs(r1 - r2) > threshold || abs(g1 - g2) > threshold || abs(b1 - b2) > threshold) {
                cout << "Fire" << endl;
                SchemaMouseEvent(ms);
            }

            ReleaseDC(NULL, hdc);
        }
    }

    return 0;
}