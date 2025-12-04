#pragma once
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

const std::string filename = "debug_log.txt";

class Debug
{
public:
    // 実行開始時に呼び出してファイルを初期化
    static void Init()
    {
        std::ofstream ofs(filename, std::ios::trunc); // 上書き
        if (ofs) ofs << "[Debug Log Started]\n";
    }

    static void Log(const std::string& msg)
    {
        std::ofstream ofs(filename, std::ios::app);
        if (!ofs) return;
        ofs << "[" << TimeString() << "] " << msg << std::endl;
    }

    template<typename T>
    static void LogValue(const std::string& name, const T& value)
    {
        std::ostringstream oss;
        oss << name << ": " << value;
        Log(oss.str());
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
