#pragma once

#include <Windows.h>
#include <iostream>

namespace Utils {
    static std::string WideCharToMultiByteString(const std::wstring& s) {
        if (s.empty()) {
            return "";
        }

        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (sizeNeeded <= 0) {
            return "";
        }

        std::string result(sizeNeeded - 1, '\0');

        WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &result[0], sizeNeeded, nullptr, nullptr);
        return result;
    }

    static std::wstring MultiByteToWideCharString(const std::string& s) {
        if (s.empty()) {
            return L"";
        }

        int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
        if (sizeNeeded <= 0) {
            return L"";
        }

        std::wstring result(sizeNeeded - 1, L'\0');

        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &result[0], sizeNeeded);
        return result;
    }

    static void SetupConsole() {
        if (AllocConsole()) {
            FILE* pConsole;

            freopen_s(&pConsole, "CONOUT$", "w", stdout);
            freopen_s(&pConsole, "CONOUT$", "w", stderr);
            freopen_s(&pConsole, "CONIN$", "r", stdin);

            std::ios_base::sync_with_stdio(true);
            std::cout.clear();
            std::wcout.clear();
            std::cin.clear();
            std::wcin.clear();

            SetConsoleOutputCP(CP_UTF8);
        }
    }
}