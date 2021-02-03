#pragma once
#include <string>

namespace Influx
{
    std::wstring s2w(std::string s)
    {
        std::wstring ws;
        std::wstring wsTmp(s.begin(), s.end());
        ws = wsTmp;
        return ws;
    }
    std::string w2s(std::wstring ws)
    {
        // [TODO]
        return std::string();
    }
}