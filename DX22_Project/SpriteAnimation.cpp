#include "SpriteAnimation.h"
#include "CanvasUI.h"
#include "GameObject.h"

void SpriteAnimation::Start()
{
    ui_ = gameObject->TryGetComponent<CanvasUI>();
    if (!ui_) return;

    dx_ = 1.0f / static_cast<float>(columns_);
    dy_ = 1.0f / static_cast<float>(rows_);

    Vector2 uvSize{ dx_, dy_ };
    ui_->SetUVScale(uvSize);

    currentFrame_ = 0;
    timer_ = 0.0f;
    playing_ = false;

    SetSpriteUV();
}

void SpriteAnimation::OnDestroy()
{
    ui_ = nullptr;
}

void SpriteAnimation::Update()
{
    if (!ui_)      return;
    if (!playing_) return;

    constexpr float fixedDt = 1.0f / 60.0f;
    timer_ += fixedDt;

    if (timer_ >= speed_)
    {
        // —]‚è‚ðŽc‚µ‚Ä‚¨‚­‚Æ‘¬“x‚ð•Ï‚¦‚½‚Æ‚«‚à”j’]‚µ‚É‚­‚¢
        timer_ -= speed_;

        ++currentFrame_;

        if (currentFrame_ >= loopFrame_)
        {
            if (isLoop_)
            {
                currentFrame_ = 0;
            }
            else
            {
                currentFrame_ = 0;
                playing_ = false; // 1Žü‚µ‚½‚ç’âŽ~
            }
        }

        SetSpriteUV();
    }
}

void SpriteAnimation::Play(bool resetFrame)
{
    playing_ = true;
    if (resetFrame)
    {
        currentFrame_ = 0;
        timer_ = 0.0f;
        SetSpriteUV();
    }
}

void SpriteAnimation::SetSpriteUV()
{
    if (!ui_) return;

    int frame = currentFrame_;
    if (frame < 0)               frame = 0;
    if (frame >= loopFrame_)     frame = loopFrame_ - 1;

    const int col = frame % columns_;
    const int row = frame / columns_;

    Vector2 uvPos{
        static_cast<float>(col) * dx_,
        static_cast<float>(row) * dy_
    };

    ui_->SetUVPos(uvPos);
}
