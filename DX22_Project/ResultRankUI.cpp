#include "ResultRankUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include "GameState.h"

constexpr Vector2 kNumberUVScale = { 1.f, 1.0f };

void ResultRankUI::Awake()
{
	// クリアタイムからランクを計算
	rank_ = CalcRank();

	// ランク画像の生成
	TextureTag rankTex = LoadRankTexture(rank_);

	GameObject& rankUI = gameObject->CreateChild();
	ui_ = &rankUI.AddComponent<UiSpriteRenderer>(rankTex, Vector2::zero(), Vector2::WindowSize());

	// ランクフラッシュ画像の生成
	GameObject& flashObj = gameObject->CreateChild();
	uiFlash_ = &flashObj.AddComponent<UiSpriteRenderer>(rankTex, Vector2::zero(), Vector2::WindowSize());
	// 最初は透明
	uiFlash_->SetColor({ 2.0f, 2.0f, 2.0f, 0.0f }); // 輝度を高めに設定

}
void ResultRankUI::Start()
{
	waitTimer_.Start();
	isRankFlashOn_ = false;
	glowTimer = 0.0f;
}

void ResultRankUI::Update()
{
	// 既に演出が終わっていたら何もしない
	if (isRankFlashOn_) return;

	//指定時間待つ
	if (waitTimer_.ElapsedRawSeconds() < WAIT_TIME)
	{
		if (uiFlash_) uiFlash_->SetVisible(false);
		return;
	}

	//待機時間が終わったら光らせる
	glowTimer += 1.0f / 60.0f;
	float t = glowTimer / kGlowDuration; // 0.0 -> 1.0

	if (t <= 1.0f)
	{
		float alpha = std::sin(t * 3.1415f); // 0 -> 1 -> 0
		if (uiFlash_)
		{
			uiFlash_->SetVisible(true);
			uiFlash_->SetColor({ 2.0f, 2.0f, 2.0f, alpha });
		}
	}
	else
	{
		// 演出終了
		isRankFlashOn_ = true;
		if (uiFlash_) uiFlash_->SetVisible(false);
		waitTimer_.Stop();
	}
}

void ResultRankUI::OnDestroy()
{
	ui_ = nullptr;
	uiFlash_ = nullptr;
}


RankType ResultRankUI::CalcRank()
{
	float time = GetRecentClearTimes();

	// S: 0~60秒 A: 61~120秒 B: 121~180秒 C: 181秒〜
	if (time <= 60.f)
	{
		return RankType::Rank_S;
	}
	else if (time <= 120.f)
	{
		return RankType::Rank_A;
	}
	else if (time <= 180.f)
	{
		return RankType::Rank_B;
	}
	else
	{
		return RankType::Rank_C;
	}
}

TextureTag ResultRankUI::LoadRankTexture(RankType rank)
{
	switch (rank)
	{
	case RankType::Rank_S:
		TextureManager::Load(TextureTag::Result_Rank_S);
		return TextureTag::Result_Rank_S;
		break;

	case RankType::Rank_A:
		TextureManager::Load(TextureTag::Result_Rank_A);
		return TextureTag::Result_Rank_A;
		break;

	case RankType::Rank_B:
		TextureManager::Load(TextureTag::Result_Rank_B);
		return TextureTag::Result_Rank_B;
		break;

	case RankType::Rank_C:
		TextureManager::Load(TextureTag::Result_Rank_C);
		return TextureTag::Result_Rank_C;
		break;
	default:
		assert(false && "Invalid RankType");
		break;
	}

	// ここには来ないはず
	assert(false && "Should not reach here");
	return TextureTag();
}