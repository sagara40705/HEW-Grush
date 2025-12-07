#pragma once
#include "Component.h"
#include "TextureManager.h"
#include "Vector2.h"
#include "Vector3.h"

// ================================================================
// BreakUiRenderer
// ---------------------------------------------------------------
// ・UI（ボタン）が「割れるアニメーション」を行うための Renderer コンポーネント
// ・UiSpriteRenderer とほぼ同じ流れで描画を行うが、使用するシェーダーが異なる
// ・3枚のテクスチャを使用する
//     ① 通常（割れる前）のボタン
//     ② ひび割れたボタン
//     ③ ピースID画像（破片ごとに色分けされた画像）
// ・StartBreak() を呼ぶと割れるアニメーションが開始
// ・Update() で破片アニメの進行度 breakT を進める
// ・Render() で「割れ専用シェーダー」を使って描画する
// ================================================================
class BreakUiRenderer : public Renderer
{
private:

    // UI の表示位置
    Vector2 m_pos;

    // UI の大きさ（幅・高さ）
    Vector2 m_size;

    // アニメーション進行度（0.0 ～ 1.0）
    float m_breakT = 0.0f;

    // 割れアニメーションが開始しているかどうか
    bool m_isBreaking = false;

    // アニメが進む速さ（1フレームあたりの増加量）
    float m_speed = 0.03f;

    // 使用するテクスチャ（タグ）
    TextureTag m_baseTag;
    TextureTag m_crackTag;
    TextureTag m_idTag;

    // Texture の実体
    Texture* m_baseTex = nullptr;
    Texture* m_crackTex = nullptr;
    Texture* m_idTex = nullptr;

public:

    // ------------------------------------------------------------
    // コンストラクタ
    // ------------------------------------------------------------
    BreakUiRenderer(
        const Vector2& pos,
        const Vector2& size,
        TextureTag baseTag,
        TextureTag crackTag,
        TextureTag idTag
    );

    // Awake：AddComponent された直後に1回だけ呼ばれる
    virtual void Awake() override;

    // Start：最初のフレームで1回呼ばれる
    virtual void Start() override;

    // Update：アニメーションの進行を行う（毎フレーム呼ばれる）
    virtual void Update();

    // Render：割れるシェーダーで実際に描画を行う
    virtual void Render() override;

    // StartBreak：外部から呼び出されると割れアニメが開始する
    void StartBreak();
};