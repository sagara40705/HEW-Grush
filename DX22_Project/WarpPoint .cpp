#include "WarpPoint.h"
#include "GameObject.h"
#include "Transform.h"

// ワープ先の座標
constexpr Vector3 kWarpPos[ToIndex(Point::Count)] = {
	{0.f, 5.f, 0.f}, // toB
	{0.f, 0.f, 0.f}, // toA
};

//初期化処理
void WarpPoint::Start()
{
	// エフェクト設定・再生
	// エフェクトが勝手にループ再生します
}

// 終了処理
void WarpPoint::OnDestroy()
{
	// エフェクトの後片付け
}

// ギミック
void WarpPoint::OnTriggerEnter(const TriggerInfo& info)
{
	/* 参考ファイル ： Component.h(114行～), GameObject.h(46, 103行目), Transform.h(31行目) */

	//プレイヤーと当たった時だけ処理

	//プレイヤーのtransformのposを変える

	//将来: エフェクトやサウンドをいれる
}
