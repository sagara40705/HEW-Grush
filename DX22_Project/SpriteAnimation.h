#pragma once
#include "Component.h"
#include "Vector2.h"


// スプライトシートアニメーション用コンポーネント
// ・1枚のテクスチャを (columns × rows) に分割
// ・UV を切り替えてアニメーションさせる
class SpriteAnimation final : public MonoBehaviour
{
private:
    // 対象レンダラー
    UiSpriteRenderer* uiRenderer = nullptr;
    Sprite3DRenderer* sprite3DRenderer = nullptr;

    // スプライトシート設定
    int columns = 1;   // 横分割数
    int rows = 1;   // 縦分割数
    int frameCount = 1;   // コマ数（0〜frameCount-1 を使用）
    int frameInterval = 1;   // 何フレームごとに次のコマへ進むか
    bool loop = true;// ループ再生するか

    // アニメーション状態
    int startFrame = 0;
    bool play = false;
    int currentFrame = 0;   // 現在のコマ番号
    int frameTimer = 0;   // 経過フレームカウンタ

    // 1 コマあたりの UV サイズ (1/columns, 1/rows)
    Vector2 uvCellSize = { 1.0f, 1.0f };

public:
    // コンストラクタ
    // cols: 横分割数, rows: 縦分割数
    // interval: 次のコマに進むまでのフレーム数
    // count: 1ループの総コマ数
    // doLoop: ループ再生するか
    SpriteAnimation(int cols, int rows, int interval, int count, bool doLoop);

    void Awake() override;
    void Start() override;
    void Update() override;

    // 再生制御（必要に応じて拡張）
    void Play();                     // 0 から開始
    void Stop();                     // 停止
    void Pause();                    // 必要なら
    void PlayFrom(int frame);        // 任意フレームから開始
    void SetFrame(int frame);        // 再生はしない（静止表示）
    void SetLoop(bool v) { loop = v; }
    void Reset()
    {
        currentFrame = 0;
        frameTimer = 0;
        ApplyFrame();
    }
    void SetStartFrame(int start) { startFrame = start; }
    void SetFramCount(int count) { frameCount = count; }
private:
    // currentFrame から UV を計算して適用
    void ApplyFrame();
};
