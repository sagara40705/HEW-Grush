#pragma once
#include "DebugLog.h"
class GameObject;

//基底コンポーネントクラス
class Component
{
public:
	GameObject* gameObject = nullptr;	//所有者
private:
	bool enabled = true;			//有効
private:
	bool onStart = false;
	bool onDestroy = false;
public:
	virtual ~Component() = default;
public:
	void RootStart();
	void RootOnDestroy();
protected:
	virtual void Start() {};
	virtual void OnDestroy() {};
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