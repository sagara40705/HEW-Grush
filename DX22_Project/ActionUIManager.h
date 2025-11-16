#pragma once

// 前方宣言
class PlayerControll; class CanvasUI;

class ActionUIManager : public MonoBehaviour
{
private:
	enum Action
	{
		Spining,
		Dash,
		Count
	};
private:
	CanvasUI* ui[Action::Count]{};
	PlayerControll* pc = nullptr;
public:
	ActionUIManager(CanvasUI* spin, CanvasUI* dash);
	void Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	const Vector2 kNumberPos[Action::Count] = { {1020.f, 520.f},{1070.f, 520.f} };	//位置(四つ分)
	const Vector2 kNumberSize = { 0.5f, 1.0f };											//縮小率
	const Vector2 kNumberUvPos[Action::Count] = { {0.f, 1.f}, {0.5f, 1.f} };	//位置(四つ分)
	const Vector2 kNumberUVSize = { 0.5f, 1.f };										//UVSize
};