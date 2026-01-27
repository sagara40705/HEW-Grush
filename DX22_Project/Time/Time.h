// Time.h（抜粋）
#pragma once

namespace Time
{
    // 初期化
    void Initialize();

    // フレームの最初
    void BeginFrame();

    // 終了時
    void Shutdown();

    // timeScale 適用後の Δt
    float DeltaTime();

    // timeScale 無しの生 Δt
    float UnscaledDeltaTime();

    // 起動からの累積時間（timeScale 適用後）
    float TimeSinceStartup();

    // 時間の進み方
    void  SetTimeScale(float scale);
    float GetTimeScale();
}
