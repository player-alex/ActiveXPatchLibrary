#include "UdpServer.h"
#include <Windows.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include "detours.h"
#include "ImhLabel.h"
#include "Utils.h"

#define UNUSED(x) (void)(x)
#pragma comment(lib, "detours.lib")

/* Constants */
constexpr int BIND_PORT = 1305;
/* ------------------------------ */

/* Variables */
static std::string oldCaption;
static std::string newCaption;
static UdpServer* udpServer;
/* ------------------------------ */

/* New Function Defs */
void STDMETHODCALLTYPE NewSetCaption(void* __this__, BSTR arg);
/* ------------------------------ */

/* Function Names */
static const char* FN_SET_CAPTION = "SetCaption";
/* ------------------------------ */

std::map<std::string, std::pair<uintptr_t, uintptr_t>> PATCH_TABLE = {
    {
        FN_SET_CAPTION,
        { 
                (uintptr_t)((BYTE*)GetModuleHandleW(L"mhLbl.dll") + ImhLabel::SetCaptionRvaOffset),
                (uintptr_t)(&NewSetCaption)
        }
    },
};

/* User Functions */
void recvCallback(const std::string& message) {
    std::cout << "Received: " <<  message << std::endl;
    newCaption = message;
}

void Initialize() {
    udpServer = new UdpServer(1305, recvCallback);
    udpServer->start();
}

void Finalize() {
    udpServer->stop();
    delete udpServer;
}
/* ------------------------------ */

/* New Functions */
void STDMETHODCALLTYPE NewSetCaption(void* __this__, BSTR arg) {
    static const auto ORG_FUNC = reinterpret_cast<void(*)(void*, BSTR)>
        (PATCH_TABLE.find(FN_SET_CAPTION)->second.first);

    oldCaption = Utils::WideCharToMultiByteString(arg);
    std::cout << "Intercepted Caption: " << oldCaption << std::endl;

    if (newCaption.length() > 0) {
        BSTR newCaptionBSTR = SysAllocString(Utils::MultiByteToWideCharString(newCaption).c_str());
        ORG_FUNC(__this__, newCaptionBSTR);
        SysFreeString(newCaptionBSTR);
    } 
    else {
        ORG_FUNC(__this__, arg);
    }
}
/* ------------------------------ */

void HookFunctions() {
    for (const auto& patchInfo : PATCH_TABLE) {
        DetourAttach(&(PVOID&)patchInfo.second.first, (PVOID)patchInfo.second.second);

        std::cout << "Patched: ["
            << patchInfo.first << "] "
            << std::hex
            << "0x" << patchInfo.second.first
            << " -> 0x" << patchInfo.second.second
            << std::endl;
    }

    DetourTransactionCommit();
}

void UnhookFunctions() {
    for (const auto& patchInfo : PATCH_TABLE) {    
        DetourDetach(&(PVOID&)patchInfo.second.first, (PVOID)patchInfo.second.second);

        std::cout << "Restored: ["
            << patchInfo.first << "] "
            << std::hex
            << "0x" << patchInfo.second.first
            << " -> 0x" << patchInfo.second.second
            << std::endl;
    }

    DetourTransactionCommit();
}

__declspec(dllexport)
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved)
{
    UNUSED(hinstDLL);
    UNUSED(lpvReserved);

    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (fdwReason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        Utils::SetupConsole();
        HookFunctions();
        Initialize();
    }
    else if (fdwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        UnhookFunctions();
        Finalize();
    }

    return TRUE;
}