#include "WarpPoint.h"
#include "GameObject.h"
#include "Transform.h"

constexpr Vector3 kWarpPos[ToIndex(Point::Count)] = {
	{0.f, 5.f, 0.f}, // toB
	{0.f, 0.f, 0.f}, // toA
};

void WarpPoint::Start()
{
	// エフェクト設定・再生
	// エフェクトが勝手にループ再生します
}

void WarpPoint::OnDestroy()
{
	// エフェクトの後片付け
}

void WarpPoint::OnTriggerEnter(const TriggerInfo& info)
{
	/* 参考ファイル ： GameObject, Transform */

	//プレイヤーと当たった時だけ処理

	//プレイヤーのtransformのposを変える

	//将来: エフェクトやサウンドをいれる
}
