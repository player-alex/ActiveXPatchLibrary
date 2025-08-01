#pragma once

#include <Windows.h>
#include <Unknwn.h>

MIDL_INTERFACE("9732FB41-C321-11D1-836F-00A0C993F125")
ImhLabel : public IUnknown
{
public:
    // Display Id: 0x60000000
    static const uintptr_t QueryInterfaceRvaOffset = 0x5982;
    virtual void STDMETHODCALLTYPE QueryInterface(
        /* [in] */ void* riid,
        /* [out] */ void* ppvObj) = 0;

    // Display Id: 0x60000001
    static const uintptr_t AddRefRvaOffset = 0x5949;
    virtual DWORD STDMETHODCALLTYPE AddRef() = 0;

    // Display Id: 0x60000002
    static const uintptr_t ReleaseRvaOffset = 0x5956;
    virtual DWORD STDMETHODCALLTYPE Release() = 0;

    // Display Id: 0x60010000
    static const uintptr_t GetTypeInfoCountRvaOffset = 0x4eff;
    virtual void STDMETHODCALLTYPE GetTypeInfoCount(/* [out] */ void* pctinfo) = 0;

    // Display Id: 0x60010001
    static const uintptr_t GetTypeInfoRvaOffset = 0x4f0e;
    virtual void STDMETHODCALLTYPE GetTypeInfo(
        /* [in] */ unsigned int itinfo,
        /* [in] */ DWORD lcid,
        /* [out] */ void* pptinfo) = 0;

    // Display Id: 0x60010002
    static const uintptr_t GetIDsOfNamesRvaOffset = 0x4f27;
    virtual void STDMETHODCALLTYPE GetIDsOfNames(
        /* [in] */ void* riid,
        /* [in] */ void* rgszNames,
        /* [in] */ unsigned int cNames,
        /* [in] */ DWORD lcid,
        /* [out] */ void* rgdispid) = 0;

    // Display Id: 0x60010003
    static const uintptr_t InvokeRvaOffset = 0x4f47;
    virtual void STDMETHODCALLTYPE Invoke(
        /* [in] */ long dispidMember,
        /* [in] */ void* riid,
        /* [in] */ DWORD lcid,
        /* [in] */ WORD wFlags,
        /* [in] */ void* pdispparams,
        /* [out] */ void* pvarResult,
        /* [out] */ void* pexcepinfo,
        /* [out] */ void* puArgErr) = 0;

    // Display Id: 0xfffffe0b
    static const uintptr_t SetBackColorRvaOffset = 0x4ca7;
    virtual void STDMETHODCALLTYPE SetBackColor(/* [in] */ IUnknown* arg) = 0;

    // Display Id: 0xfffffe0b
    static const uintptr_t GetBackColorRvaOffset = 0x4e45;
    virtual IUnknown* STDMETHODCALLTYPE GetBackColor() = 0;

    // Display Id: 0xfffffe00
    static const uintptr_t SetRefFontRvaOffset = 0x4dc7;
    virtual void STDMETHODCALLTYPE SetRefFont(/* [in] */ void* arg) = 0;

    // Display Id: 0xfffffe00
    static const uintptr_t SetFontRvaOffset = 0x4d3b;
    virtual void STDMETHODCALLTYPE SetFont(/* [in] */ void* arg) = 0;

    // Display Id: 0xfffffe00
    static const uintptr_t GetFontRvaOffset = 0x4df6;
    virtual void* STDMETHODCALLTYPE GetFont() = 0;

    // Display Id: 0xfffffdff
    static const uintptr_t SetForeColorRvaOffset = 0x4e65;
    virtual void STDMETHODCALLTYPE SetForeColor(/* [in] */ IUnknown* arg) = 0;

    // Display Id: 0xfffffdff
    static const uintptr_t GetForeColorRvaOffset = 0x4e8a;
    virtual IUnknown* STDMETHODCALLTYPE GetForeColor() = 0;

    // Display Id: 0xfffffdfa
    static const uintptr_t SetCaptionRvaOffset = 0x4c4d;
    virtual void STDMETHODCALLTYPE SetCaption(/* [in] */ BSTR arg) = 0;

    // Display Id: 0xfffffdfa
    static const uintptr_t GetCaptionRvaOffset = 0x4ed9;
    virtual BSTR STDMETHODCALLTYPE GetCaption() = 0;
};