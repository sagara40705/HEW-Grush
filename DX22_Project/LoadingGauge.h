#pragma once
#include "Component.h"
#include "Vector2.h"
#include "Vector3.h"

class LoadingScene;
class UiSpriteRenderer;

//　LoadingSceneの進捗ゲージ表示用コンポーネント
class LoadingGauge : public MonoBehaviour
{
private:
	LoadingScene* loading_ = nullptr;
	UiSpriteRenderer* ui_ = nullptr;

	Vector2 fullSize_ = Vector2::one(); // 最大サイズ
	Vector2 baseCenterPos_ = Vector2::zero();   // 初期の中心座標


public:
	explicit LoadingGauge(LoadingScene* loading);
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	// 文字のUIを教えるための関数
	
};