#include "Time.h"
#include <chrono>
#include <algorithm>
#include <limits>

namespace
{
    using Clock = std::chrono::steady_clock;
    using TimePt = Clock::time_point;
    using Seconds = std::chrono::duration<float>;

    TimePt s_prevTime{};
    float  s_delta = 0.0f; // timeScale 適用後
    float  s_unscaledDelta = 0.0f; // 生 dt
    float  s_total = 0.0f; // 起動からの累積（timeScale 適用後）
    float  s_timeScale = 1.0f;
    bool   s_initialized = false;
}

namespace Time
{
    void Initialize()
    {
        s_prevTime = Clock::now();
        s_delta = 0.0f;
        s_unscaledDelta = 0.0f;
        s_total = 0.0f;
        s_timeScale = 1.0f;
        s_initialized = true;
    }

    void BeginFrame()
    {
        if (!s_initialized)
        {
            Initialize();
        }

        const TimePt now = Clock::now();
        Seconds raw = now - s_prevTime;
        s_prevTime = now;

        float dt = raw.count();

        // 必要ならクランプ（極端なラグで物理が暴れないように）
        const float maxDt = 0.05f; // 約 20fps 下限
        if (dt > maxDt) dt = maxDt;
        if (dt < 0.0f)  dt = 0.0f;

        s_unscaledDelta = dt;
        s_delta = dt * s_timeScale;
        s_total += s_delta;
    }

    void Shutdown()
    {
        s_initialized = false;
    }

    float DeltaTime()
    {
        return s_delta;
    }

    float UnscaledDeltaTime()
    {
        return s_unscaledDelta;
    }

    float TimeSinceStartup()
    {
        return s_total;
    }

    void SetTimeScale(float scale)
    {
        s_timeScale = scale;
    }

    float GetTimeScale()
    {
        return s_timeScale;
    }
}