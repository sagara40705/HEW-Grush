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

}
void ResultRankUI::Start()
{

}

void ResultRankUI::OnDestroy()
{
	ui_ = nullptr;
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