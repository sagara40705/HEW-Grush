#pragma once
#include "DebugLog.h"
class GameObject;

//基底コンポーネントクラス
class Component
{
protected:
public:
	virtual ~Component() = default;
	GameObject* Parent = nullptr;	//所有者
	bool enabled = true;			//有効
public:
	virtual void Start() {}
	virtual void OnWillDestroy() {}
	virtual void OnDestroy() {}
public:
	void SetEnabled(bool v) { enabled = v; }
	bool IsEnabled() const { return enabled; }
};

class MonoBehaviour : public Component
{
public:
	virtual void Update() {}
	virtual void Draw() {}
};

class Transform;
class Camera;
class MeshFilter;
class MeshRender;