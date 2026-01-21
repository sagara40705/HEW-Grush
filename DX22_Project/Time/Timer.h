#pragma once
#include <chrono>
#include <algorithm>
#include <limits>

#undef min
#undef max

// ------------------------------------------------------------
// 汎用タイマー：ストップウォッチ／カウントダウン／スローモーション対応
// ------------------------------------------------------------
class Timer
{
public:
    using Clock = std::chrono::steady_clock;
    using TimePt = Clock::time_point;
    using Seconds = std::chrono::duration<double>;

    Timer() = default;

    // -------- 基本操作 --------

    // 累積リセットして計測開始
    void Start()
    {
        accumulatedRaw_ = Seconds::zero();
        startRaw_ = Clock::now();
        running_ = true;
    }

    // 一時停止（累積は保持）
    void Pause()
    {
        if (!running_) return;
        accumulatedRaw_ += Clock::now() - startRaw_;
        running_ = false;
    }

    // 再開
    void Resume()
    {
        if (running_) return;
        startRaw_ = Clock::now();
        running_ = true;
    }

    // 完全停止（累積もリセット）
    void Stop()
    {
        running_ = false;
        accumulatedRaw_ = Seconds::zero();
    }

    // リスタート（Stop → Start）
    void Restart()
    {
        Stop();
        Start();
    }

    // -------- タイムスケール --------

    // 1.0 = 通常, 0.5 = 半分, 0 = 停止
    void SetTimeScale(double scale)
    {
        timeScale_ = scale;
    }

    double GetTimeScale() const
    {
        return timeScale_;
    }

    // -------- 経過時間取得 --------

    // 生（スケールなし）の経過時間（秒）
    double ElapsedRawSeconds() const
    {
        return ElapsedRaw().count();
    }

    // スケール適用後の経過時間（秒）
    double ElapsedSeconds() const
    {
        return ElapsedRaw().count() * timeScale_;
    }

    // 現在動いているか
    bool IsRunning() const
    {
        return running_;
    }

    // -------- カウントダウン用途 --------

    // カウントダウン総時間（秒）（0以下で無効）
    void SetDuration(double seconds)
    {
        durationSec_ = seconds;
    }

    double GetDuration() const
    {
        return durationSec_;
    }

    // 残り時間（秒）（duration 無効時は +∞）
    double RemainingSeconds() const
    {
        if (durationSec_ <= 0.0)
        {
            return std::numeric_limits<double>::infinity();
        }
        const double remaining = durationSec_ - ElapsedSeconds();
        return std::max(0.0, remaining);
    }

    // カウントダウンが終了したか
    bool IsFinished() const
    {
        if (durationSec_ <= 0.0) return false;
        return ElapsedSeconds() >= durationSec_;
    }

private:
    Seconds ElapsedRaw() const
    {
        if (!running_)
        {
            return accumulatedRaw_;
        }
        return accumulatedRaw_ + (Clock::now() - startRaw_);
    }

private:
    TimePt  startRaw_{};
    Seconds accumulatedRaw_{ Seconds::zero() };
    double  timeScale_ = 1.0;
    double  durationSec_ = 0.0;
    bool    running_ = false;
};