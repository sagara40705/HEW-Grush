#include "StageClearRank.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

void StageClearRank::Awake()
{
	//// ƒNƒŠƒAƒ^ƒCƒ€‚©‚çƒ‰ƒ“ƒN‚ðŒvŽZ
	rank_ = ClearRank();

	if (!isStageRank_)
	{
		//// ƒ‰ƒ“ƒN‰æ‘œ‚Ì¶¬
		TextureTag rankTex = LoadTexture(rank_);


		GameObject& rankUI = gameObject->CreateChild();
		Stage1Rank_ = &rankUI.AddComponent<UiSpriteRenderer>(rankTex, Vector2::zero(), Vector2::WindowSize());

		Stage1Rank_->SetVisible(false);

		isStageRank_ = true;
	}
}

void StageClearRank::Start()
{
	
}

void StageClearRank::Update()
{
	
}

void StageClearRank::OnDestroy()
{

}

StageId StageClearRank::GetStageSelect()
{
	
	//Stage1Rank_->SetVisible(false);

	switch (StageSelect_)
	{
	case StageId::Stage1:
		Stage1Rank_->SetVisible(true);
		return StageId::Stage1;
		break;
	case StageId::Stage2:
		return StageId::Stage2;
		break;
	case StageId::Stage3a:
		return StageId::Stage3a;
		break;
	case StageId::Stage3b:
		return StageId::Stage3b;
		break;	
	case StageId::Stage3c:
		return StageId::Stage3c;
		break;
	case StageId::None:
		
		return StageId::None;
	default:
		assert(false && "Invalid RankType");
		break;
	}
	// ‚±‚±‚É‚Í—ˆ‚È‚¢‚Í‚¸
	assert(false && "Should not reach here");
	return StageId();
}

TextureTag StageClearRank::LoadTexture(RankType rank_)
{
	switch (rank_)
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

	// ‚±‚±‚É‚Í—ˆ‚È‚¢‚Í‚¸
	assert(false && "Should not reach here");
	return TextureTag();
}

RankType StageClearRank::ClearRank()
{
	float time = GetRecentClearTimes();

	// S: 0~60•b A: 61~120•b B: 121~180•b C: 181•b?
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
