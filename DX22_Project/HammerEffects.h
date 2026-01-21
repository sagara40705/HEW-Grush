#pragma once
#include "Component.h"

class GradeEffects : public Component
{
public:
	enum Type
	{
		GradeUp,
		Count,
	};
private:
	const int MAX_EFFECTS[Type::Count] = { 2 };
public:
	void Awake() override;
	void Start() override;
	void OnDestroy() override;
public:
	void Play(Type type);
private:
	void CreateGradeUpEffect();
};