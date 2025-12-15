#ifndef IME_HELPER_H
#define IME_HELPER_H

// Windows IME Helper - Separate from raylib to avoid conflicts
// Must be included BEFORE raylib.h in any file that uses it

#ifdef _WIN32

// Prevent raylib conflicts
#define NOGDI
#define NOUSER

#include <windows.h>
#include <imm.h>
#include <string>

#pragma comment(lib, "imm32.lib")

// Get the current IME composition string
inline std::string GetIMECompositionString(HWND hwnd)
{
    std::string result;
    HIMC hIMC = ImmGetContext(hwnd);
    if (hIMC)
    {
        // Get composition string length
        LONG len = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0);
        if (len > 0)
        {
            wchar_t *buffer = new wchar_t[len / sizeof(wchar_t) + 1];
            ImmGetCompositionStringW(hIMC, GCS_COMPSTR, buffer, len);
            buffer[len / sizeof(wchar_t)] = L'\0';

            // Convert wide string to UTF-8
            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
            if (utf8Len > 0)
            {
                char *utf8Buffer = new char[utf8Len];
                WideCharToMultiByte(CP_UTF8, 0, buffer, -1, utf8Buffer, utf8Len, NULL, NULL);
                result = utf8Buffer;
                delete[] utf8Buffer;
            }
            delete[] buffer;
        }
        ImmReleaseContext(hwnd, hIMC);
    }
    return result;
}

// Get the result string (committed text)
inline std::string GetIMEResultString(HWND hwnd)
{
    std::string result;
    HIMC hIMC = ImmGetContext(hwnd);
    if (hIMC)
    {
        LONG len = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);
        if (len > 0)
        {
            wchar_t *buffer = new wchar_t[len / sizeof(wchar_t) + 1];
            ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, buffer, len);
            buffer[len / sizeof(wchar_t)] = L'\0';

            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
            if (utf8Len > 0)
            {
                char *utf8Buffer = new char[utf8Len];
                WideCharToMultiByte(CP_UTF8, 0, buffer, -1, utf8Buffer, utf8Len, NULL, NULL);
                result = utf8Buffer;
                delete[] utf8Buffer;
            }
            delete[] buffer;
        }
        ImmReleaseContext(hwnd, hIMC);
    }
    return result;
}

#endif // _WIN32

#endif // IME_HELPER_H
