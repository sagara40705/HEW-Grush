#include "WarpPoint.h"
#include "GameObject.h"
#include "Transform.h"
#include "DebugLog.h"

// ワープ先の座標
constexpr Vector3 kWarpPos[ToIndex(Point::Count)] = {
	{55.f, 0.f, 0.f}, // toB
	{35.f, 0.f, 0.f}, // toA
	{75.f, 0.f, 0.f}, // toD 73.f, 0.f, 0.f
	{33.f, 0.f, 0.f}, // toC
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
	if (!info.other->CompareTag(Tag::Player)) {
		return;
	}

	//プレイヤーのtransformのposを変える
	Transform* playerTransform = info.other->GetComponent<Transform>();
	playerTransform->SetPosition(kWarpPos[ToIndex(point_)]);

	//将来: エフェクトやサウンドをいれる

	DebugLog::OutputValue("ワープ起動, 番号", ToIndex(point_));

}
