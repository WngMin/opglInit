#pragma once

#include <cassert>
#include <string>
#include <Windows.h>
#include <comdef.h>

inline std::wstring AnsiToWString(const std::string& str)
{
    WCHAR buffer[512];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
    return std::wstring(buffer);
}

class HresultException
{
    HresultException() = default;
    HresultException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

    std::wstring ToString()const;

    HRESULT ErrorCode = S_OK;
    std::wstring FunctionName;
    std::wstring Filename;
    int LineNumber = -1;
};

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                                   \
{                                                                          \
    HRESULT hr__ = (x);                                                    \
    std::wstring wfn = AnsiToWString(__FILE__);                            \
    if(FAILED(hr__)) { throw HresultException(hr__, L#x, wfn, __LINE__); } \
}
#endif