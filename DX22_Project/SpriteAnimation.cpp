#include "SpriteAnimation.h"
#include "UiSpriteRenderer.h"
#include "GameObject.h"

SpriteAnimation::SpriteAnimation(int cols, int rows, int interval, int count, bool doLoop) : columns(cols), rows(rows), frameCount(count), frameInterval(interval), loop(doLoop)
{
}

void SpriteAnimation::Awake()
{
    // 入力値の安全化
    if (columns <= 0) columns = 1;
    if (rows <= 0) rows = 1;
    if (frameInterval <= 0) frameInterval = 1;
    if (frameCount <= 0) frameCount = columns * rows;

    // 1コマの UV サイズ
    uvCellSize.x = 1.0f / static_cast<float>(columns);
    uvCellSize.y = 1.0f / static_cast<float>(rows);

    currentFrame = 0;
    frameTimer = 0;
   
}

void SpriteAnimation::Start()
{
    uiRenderer = gameObject->GetComponent<UiSpriteRenderer>();
    ApplyFrame();
}

void SpriteAnimation::Update()
{
    // 対象が無ければ何もしない
    if (!uiRenderer /* && !spriteRenderer3D */) return;
    if (!play)  return;

    // コマ送りタイミング管理
    ++frameTimer;
    if (frameTimer < frameInterval)
        return;

    frameTimer = 0;
    ++currentFrame;

    if (currentFrame >= frameCount)
    {
        if (loop)
        {
            currentFrame = 0;
        }
        else
        {
            play = false;
            currentFrame = frameCount - 1; // 最終コマで停止
            return;
        }
    }

    ApplyFrame();
}

//開始
void SpriteAnimation::Play()
{
    play = true;
}

//完全停止
void SpriteAnimation::Stop()
{
    play = false;
    Reset();
}

//途中停止
void SpriteAnimation::Pause()
{
    play = false;
}

//指定開始
void SpriteAnimation::PlayFrom(int frame)
{
    play = true;
    
    SetFrame(frame);
}

void SpriteAnimation::SetFrame(int frame)
{
    // 範囲チェック（次の項目で詳しく）
    if (frame < 0) frame = 0;
    if (frame >= frameCount) frame = frameCount - 1;

    currentFrame = frame;
    frameTimer = 0;
    ApplyFrame();
}

void SpriteAnimation::ApplyFrame()
{
    if (!uiRenderer /* && !spriteRenderer3D */) return;

    // currentFrame → (col, row) へ変換（左上から右→下）
    const int col = currentFrame % columns;
    const int row = currentFrame / columns;

    Vector2 uvPos= {0.f, 0.f};
    uvPos.x = uvCellSize.x * static_cast<float>(col);
    uvPos.y = uvCellSize.y * static_cast<float>(row);

    // UI 用
    if (uiRenderer)
        uiRenderer->SetUV(uvPos, uvCellSize);

    // 3D 用 SpriteRenderer を実装したらここで同様に UV を適用すればよい
    // if (spriteRenderer3D)
    //     spriteRenderer3D->SetUV(uvPos, uvCellSize);
}
