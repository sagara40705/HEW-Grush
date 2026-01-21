#pragma once
#include <chrono>


// ------------------------------------------------------------
// シンプルなストップウォッチ：クールダウンや経過時間チェック用
// ------------------------------------------------------------
class Stopwatch
{
public:
    using Clock = std::chrono::steady_clock;
    using TimePt = Clock::time_point;
    using Seconds = std::chrono::duration<double>;

    Stopwatch()
    {
        Reset();
    }

    // 計測開始（基準時刻を今にする）
    void Reset()
    {
        start_ = Clock::now();
    }

    // 現在までの経過秒（リセットしない）
    double ElapsedSeconds() const
    {
        return std::chrono::duration_cast<Seconds>(
            Clock::now() - start_
        ).count();
    }

    // 前回からの経過秒を返して、基準時刻を今に更新
    // フレーム dt 用や、定期処理用に便利
    double Restart()
    {
        const TimePt now = Clock::now();
        const double dt = std::chrono::duration_cast<Seconds>(
            now - start_
        ).count();
        start_ = now;
        return dt;
    }

private:
    TimePt start_{};
};