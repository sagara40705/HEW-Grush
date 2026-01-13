#pragma once
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <d3d11.h>  

const std::string filename = "debug_log.txt";

class DebugLog
{
public:
    // 実行開始時に呼び出してファイルを初期化
    static void Init()
    {
        std::ofstream ofs(filename, std::ios::trunc); // 上書き
        if (ofs) ofs << "[Debug Log Started]\n";
    }

    static void Uninit(ID3D11Device* device)
    {
#if defined(_DEBUG)
        if (device)
        {
            ID3D11Debug* debug = nullptr;
            if (SUCCEEDED(device->QueryInterface(__uuidof(ID3D11Debug),
                reinterpret_cast<void**>(&debug))))
            {
                // 残っている D3D11 リソースを詳細付きで出力
                debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
                debug->Release();
            }
        }
#endif

        // ついでにログファイルのフッタを書いてもよい
        std::ofstream ofs(filename, std::ios::app);
        if (ofs) ofs << "[Debug Log End]\n";
    }

    static void Output(const std::string& msg)
    {
        std::ofstream ofs(filename, std::ios::app);
        if (!ofs) return;
        ofs << "[" << TimeString() << "] " << msg << std::endl;
    }

    template<typename T>
    static void OutputValue(const std::string& name, const T& value)
    {
        std::ostringstream oss;
        oss << name << ": " << value;
        Output(oss.str());
    }

private:
    static std::string TimeString()
    {
        std::time_t t = std::time(nullptr);
        std::tm tm{};
        localtime_s(&tm, &t);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%H:%M:%S", &tm);
        return buf;
    }
};