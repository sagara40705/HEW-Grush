//#include "BreakButtonRenderer.h"
//#include "Canvas.h"
//#include "ShaderList.h"
//#include "Sprite.h"
//#include "DirectX.h"
//
//using namespace DirectX;
//
//// ================================================================
//// コンストラクタ
//// ================================================================
//BreakUiRenderer::BreakUiRenderer(
//    const Vector2& pos,
//    const Vector2& size,
//    TextureTag baseTag,
//    TextureTag crackTag,
//    TextureTag idTag
//)
//    : m_pos(pos)
//    , m_size(size)
//    , m_baseTag(baseTag)
//    , m_crackTag(crackTag)
//    , m_idTag(idTag)
//{
//}
//
//// ================================================================
//// Awake：AddComponent 直後に呼ばれる
//// ================================================================
//void BreakUiRenderer::Awake()
//{
//    // 特に処理なし（必要であればシェーダーの取得などを行う）
//}
//
//// ================================================================
//// Start：最初のフレームで呼ばれる
//// ================================================================
//void BreakUiRenderer::Start()
//{
//    // TextureManager から3枚のテクスチャを取得
//    m_baseTex = TextureManager::Get(m_baseTag);
//    m_crackTex = TextureManager::Get(m_crackTag);
//    m_idTex = TextureManager::Get(m_idTag);
//}
//
//// ================================================================
//// Update：毎フレーム呼ばれる（割れアニメの進行）
//// ================================================================
//void BreakUiRenderer::Update()
//{
//    // アニメが開始されていない場合は何もしない
//    if (!m_isBreaking) return;
//
//    // 進行度を加算
//    m_breakT += m_speed;
//
//    // 1.0 を超えないようにする
//    if (m_breakT > 1.0f)
//        m_breakT = 1.0f;
//}
//
//// ================================================================
//// Render：割れるシェーダーによる描画
//// ================================================================
//void BreakUiRenderer::Render()
//{
//    // 可視状態でなければ描画しない
//    if (!IsVisible()) return;
//
//    // テクスチャが揃っていなければ描画不可
//    if (!m_baseTex || !m_crackTex || !m_idTex) return;
//
//    // -------------------------------
//    // 1) WVP 行列の準備（UiSpriteRenderer と同じ）
//    // -------------------------------
//    Canvas* canvas = Canvas::GetMain();
//    if (!canvas) return;
//
//    XMMATRIX W = XMMatrixIdentity();
//    XMMATRIX V = XMMatrixIdentity();
//    XMMATRIX P = XMLoadFloat4x4(&canvas->GetProjMatrix());
//    W = XMMatrixTranspose(W);
//    V = XMMatrixTranspose(V);
//    P = XMMatrixTranspose(P);
//
//    struct WVP_CB
//    {
//        XMFLOAT4X4 world;
//        XMFLOAT4X4 view;
//        XMFLOAT4X4 proj;
//    } wvp;
//
//    XMStoreFloat4x4(&wvp.world, W);
//    XMStoreFloat4x4(&wvp.view, V);
//    XMStoreFloat4x4(&wvp.proj, P);
//
//    // -------------------------------
//    // 2) 割れ用シェーダーのパラメータ
//    // -------------------------------
//    struct BreakParamCB
//    {
//        // Sprite と同じ offset/size を入れておく
//        XMFLOAT4 offset_size; // (x, y, w, h)
//        float breakT;         // 割れアニメの進行度
//        XMFLOAT3 pad;         // サイズ合わせ
//    };
//
//    BreakParamCB bp;
//    bp.offset_size = { m_pos.x, m_pos.y, m_size.x, m_size.y };
//    bp.breakT = m_breakT;
//
//    // -------------------------------
//    // 3) シェーダーを取得（※あなたが作る「割れUI専用 PS」）
//    // -------------------------------
//    auto* vs = ShaderList::GetVS(ShaderList::VS_SPRITE);   // Sprite と同じでOK
//    auto* ps = ShaderList::GetPS(ShaderList::PS_BREAKUI);  // 新しく追加する必要がある
//
//    if (!vs || !ps) return;
//
//    // -------------------------------
//    // 4) 定数バッファ（cbuffer）に書き込む
//    // -------------------------------
//    vs->WriteBuffer(0, &wvp);
//    vs->WriteBuffer(1, &bp);
//    ps->WriteBuffer(1, &bp);
//
//    // -------------------------------
//    // 5) テクスチャを 0,1,2 番スロットに設定
//    // -------------------------------
//    ps->SetTexture(0, m_baseTex);   // 通常ボタン
//    ps->SetTexture(1, m_crackTex);  // 割れボタン
//    ps->SetTexture(2, m_idTex);     // IDマップ
//
//    SetDepthTest(false);
//    SetCullingMode(D3D11_CULL_NONE);
//
//    // -------------------------------
//    // 6) シェーダーをバインドして描画（Quad）
//    // -------------------------------
//    vs->Bind();
//    ps->Bind();
//
//    Sprite::Draw();
//}
//
//// ================================================================
//// StartBreak：外部から呼んで割れアニメ開始
//// ================================================================
//void BreakUiRenderer::StartBreak()
//{
//    m_isBreaking = true;
//    m_breakT = 0.0f;
//}
