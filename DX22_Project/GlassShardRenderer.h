#pragma once
#include "Component.h"
#include "GlassObject.h"      // GlassGrade
#include <vector>
#include <DirectXMath.h>

class GlassShardRenderer : public Renderer//, public MonoBehaviour
{
public:
    // 1 破片分の描画情報
    struct ShardDrawData
    {
        DirectX::XMMATRIX      world;      // 破片のワールド行列
        DirectX::XMFLOAT4      randParam;  // VS_SHARD 用ランダムパラメータ
        GlassGrade             grade;      // ガラスのグレード
    };

public:
    GlassShardRenderer() = default;
    ~GlassShardRenderer() override = default;

    void Start() override;
    void OnDestroy() override;
    //void Update()  override;
    void Render() override;

    // シングルトン取得
    static GlassShardRenderer* Instance() { return s_instance_; }

   
    // 1 個分の描画登録
    static void Push(const ShardDrawData& d);

private:
    //唯一の実体
    static GlassShardRenderer* s_instance_;

    // 今フレームの破片一覧
    std::vector<ShardDrawData> list_;     

public:
    // フレーム先頭で呼び出してバッファをクリア
    void BeginFrame();

    // 内部描画処理
    void DrawAll();                       
};
