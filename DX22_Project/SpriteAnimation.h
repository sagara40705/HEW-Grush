#pragma once
#include "Component.h"
#include "Vector2.h"

class CanvasUI;

class SpriteAnimation : public MonoBehaviour
{
private:
    CanvasUI* ui_ = nullptr;

    int   rows_ = 1;
    int   columns_ = 1;
    int   loopFrame_ = 1;    // 使用するフレーム数
    float speed_ = 1.0f; // 1フレームあたりの秒数
    bool  isLoop_ = false;

    int   currentFrame_ = 0;
    float timer_ = 0.0f;

    float dx_ = 0.0f; // 1コマ分のUV幅 (1 / columns_)
    float dy_ = 0.0f; // 1コマ分のUV高さ (1 / rows_)

    bool  playing_ = true;

public:
    // rows : 縦分割数, columns : 横分割数
    // frameCount : 使用フレーム数 (rows*columns 以内)
    // frameTime  : 1コマの表示時間(秒)
    // loop       : ループ再生するか
    SpriteAnimation(int rows, int columns, int frameCount, float frameTime, bool loop)
        : rows_(rows)
        , columns_(columns)
        , loopFrame_(frameCount)
        , speed_(frameTime)
        , isLoop_(loop)
    {
    }

    void Start() override;
    void OnDestroy() override;
    void Update() override;

    // 再生制御（必要に応じて使用）
    void Play(bool resetFrame = true);
    void Stop() { playing_ = false; }
    bool IsPlaying()const { return playing_; }

private:
    void SetSpriteUV();
};
