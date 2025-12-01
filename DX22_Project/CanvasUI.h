#pragma once

// ---------------------------------------------
// CanvasUI
// 2D UI（画像）を画面に描画するためのコンポーネントクラス
// スプライトの位置・サイズ・UV・色などをまとめて管理する
// ---------------------------------------------

#include "Component.h"   // MonoBehaviour / コンポーネント基底クラス
#include "Vector2.h"     // 2D ベクトル (x, y)
#include <memory>        // std::shared_ptr
#include "MeshBuffer.h"  // メッシュデータ管理（頂点バッファなど）
#include "Shader.h"      // シェーダ基底クラス
#include "Texture.h"     // テクスチャクラス

// MonoBehaviour を継承することで、Start / Draw / OnDestroy などの
// ライフサイクルイベントを利用できる
class CanvasUI : public MonoBehaviour
{
public:
    // ---------------------------------------------------------
    // コンストラクタ
    //  Texture* texture : 描画するテクスチャ
    //  Vector2 pos      : 画面上の表示位置（左上座標、ピクセル）
    //  Vector2 size     : テクスチャに対する比率 (1.0f = テクスチャそのままのサイズ)
    //  Vector2 uvPos    : UV の開始位置（0.0～1.0）
    //  Vector2 uvSize   : UV のスケール（0.0～1.0）
    //
    //  例) size = (1,1) : テクスチャ全体をそのまま表示
    //      size = (0.5,0.5) : テクスチャの半分サイズで表示
    // ---------------------------------------------------------
    CanvasUI(
        Texture* texture,
        Vector2 pos = Vector2::zero(),  // デフォルトは画面左上
        Vector2 size = Vector2::one(),   // デフォルトはテクスチャそのまま
        Vector2 uvPos = Vector2::zero(),  // デフォルトはUV左上(0,0)
        Vector2 uvSize = Vector2::one()    // デフォルトはUV全域(1,1)
    );

    // ---------------------------------------------------------
    // MonoBehaviour からのオーバーライド
    // ---------------------------------------------------------

    // Start()
    // オブジェクト生成後、一度だけ呼ばれる初期化処理
    // メッシュの作成やシェーダの用意などを行う
    void Start() override;

    // OnDestroy()
    // オブジェクト削除時に呼ばれる終了処理
    // ポインタ解放や参照クリアなどを行う
    void OnDestroy() override;

    // Draw()
    // 毎フレームの描画処理
    // 行列の更新 → シェーダバインド → メッシュ描画の流れ
    void Draw() override;

public:

<<<<<<< HEAD
    // ---------------------------------------------------------
    // CanvasUI 内部で使用する描画用データ構造体
    // CPU 側がまとめて保持し、シェーダに送る
    // ---------------------------------------------------------
    struct Data
    {
        // UI 用の四角形メッシュ
        std::shared_ptr<MeshBuffer> mesh;

        // シェーダに送る行列
        // matrix[0] = world 行列
        // matrix[1] = view 行列
        // matrix[2] = projection 行列
        DirectX::XMFLOAT4X4 matrix[3];

        // シェーダに送るパラメータ
        // param[0] = (offsetX, offsetY, widthPX, heightPX)
        // param[1] = (uvPosX, uvPosY, uvScaleX, uvScaleY)
        // param[2] = (r, g, b, a) カラー
        DirectX::XMFLOAT4 param[3];

        // 使用するテクスチャ（スプライト画像）
        Texture* texture = nullptr;

        // 実際に使用する頂点シェーダ / ピクセルシェーダ
        // デフォルトは静的メンバ s_defVS / s_defPS が入る
        VertexShader* vs = nullptr;
        PixelShader* ps = nullptr;
    };

    // CanvasUI が使う実データインスタンス
    Data m_data;

    // ※元々は Texture 所有フラグがあった模様
    // bool texOwner = false;

=======
	struct Data
	{
		std::shared_ptr<MeshBuffer> mesh;
		DirectX::XMFLOAT4X4 matrix[3];
		DirectX::XMFLOAT4 param[3];
		Texture* texture;
		VertexShader* vs;
		PixelShader* ps;
	};
	Data m_data;
	//bool onAnimetion = false;
	//bool texOwner = false;
>>>>>>> c0256b698a1e3b019df57ea9df352b2b94c197cf
public:
    // ==============================
    // UI パラメータの設定用 メソッド群
    // ==============================

    // -----------------------------------------
    // SetOffset(Vector2 offset)
    // UI の左上座標（画面上の位置）を設定
    // offset.x : 画面左からのピクセル数
    // offset.y : 画面上からのピクセル数
    // -----------------------------------------
    void SetOffset(Vector2 offset);

    // X 座標だけ変更
    void SetPosX(float x);

    // Y 座標だけ変更
    void SetPosY(float y);

    // -----------------------------------------
    // SetSize(Vector2 size)
    // 描画サイズをテクスチャに対する比率で設定
    //
    // 実際のピクセルサイズは SetParam() 内で
    //  texW * size.x, texH * size.y として決まる
    // -----------------------------------------
    void SetSize(Vector2 size);

    // -----------------------------------------
    // SetUVPos(Vector2 pos)
    // UV の開始位置（0.0～1.0）を設定
    // スプライトシートの切り出し開始位置などに利用
    // -----------------------------------------
    void SetUVPos(Vector2 pos);

    // -----------------------------------------
    // SetUVScale(Vector2 scale)
    // UV の拡大率（0.0～1.0）を設定
    // 例) (0.5, 1.0) → 横方向に半分だけ利用
    // -----------------------------------------
    void SetUVScale(Vector2 scale);

    // -----------------------------------------
    // SetColor(XMFLOAT4 color)
    // テクスチャ色に乗算するカラーを設定
    // color = (r, g, b, a)
    // 例) (1,1,1,1) : 変更なし / 完全不透明
    //     (1,1,1,0.5) : 半透明
    //     (1,0,0,1) : 赤色に着色
    // -----------------------------------------
    void SetColor(DirectX::XMFLOAT4 color);

    // -----------------------------------------
    // SetVertexShader / SetPixelShader
    // 将来的に 外部からシェーダ差し替えをしたい場合用
    // 現状 .cpp 側では未実装（空関数）
    // ※必要になったら実装を追加して使う想定
    // -----------------------------------------
    void SetVertexShader(Shader* vs);
    void SetPixelShader(Shader* ps);

private:
    // ==============================
    // 内部処理用 メソッド群
    // ==============================

    // -----------------------------------------
    // CreateMesh()
    // UI 用の四角形メッシュを作成
    // (0,0)-(1,1) の 1x1 サイズの矩形を作り、
    // シェーダ側で size・offset を掛けて画面座標へ
    // -----------------------------------------
    void CreateMesh();

    // -----------------------------------------
    // ensureDefaultShaders()
    // デフォルトの頂点シェーダ / ピクセルシェーダを
    // まだ作っていなければコンパイルして静的メンバに保持
    //
    // すべての CanvasUI インスタンスで共有される
    // -----------------------------------------
    void ensureDefaultShaders();

    // -----------------------------------------
    // SetMatrix()
    // UI 描画用の行列を設定
    // world = I, view = I, proj = Orthographic(0～画面幅, 0～画面高さ)
    // とすることで「1px = 1unit」で扱えるようにしている
    // -----------------------------------------
    void SetMatrix();

    // -----------------------------------------
    // SetParam()
    // テクスチャの実際の幅・高さを取得し、
    // 比率で指定していた size をピクセルサイズに変換する
    // param[0] = (offsetX, offsetY, widthPX, heightPX)
    // -----------------------------------------
    void SetParam();

    // -----------------------------------------
    // デフォルトシェーダ（全 CanvasUI で共有）
    // s_defVS : UI 専用頂点シェーダ
    // s_defPS : UI 専用ピクセルシェーダ
    // -----------------------------------------
    static std::shared_ptr<VertexShader> s_defVS;
    static std::shared_ptr<PixelShader>  s_defPS;
};