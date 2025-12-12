#pragma once
#include "Component.h"
#include "GameObject.h"

//生成管理の数
constexpr int kSpawnDataCount = 5;

// ガラスオブジェクトスポーナー
// 一定間隔でガラスオブジェクトをスポーンするコンポーネント
class GlassObjectSpawner : public MonoBehaviour
{
private:
	// メンバ変数
	const float spawnInterval = 10.0f; // スポーン間隔
	float spawnTimer = 0.0f; // 最後のスポーンからの経過時間(タイマー)
	Camera* cam_ = nullptr;

	//スポーンさせるGameOnjectのID
	ObjectId spawnObjectId[kSpawnDataCount] = {};
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};