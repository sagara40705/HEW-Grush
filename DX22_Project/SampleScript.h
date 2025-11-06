#pragma once
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshFilter.h"

class SampleScript : public Component
{
public:
    // 参照キャッシュ
    Transform* t = nullptr;
    Camera* c = nullptr;
    MeshFilter* m = nullptr;

public:
    void Start() override
    {
        Debug::Log("SampleScript::Start");
        if (Parent)
        {
            t = Parent->TryGetComponent<Transform>();
            if (t)
            {
                Debug::LogValue("cube pos x", t->GetPosition().x);
                Debug::LogValue("cube pos y", t->GetPosition().y);
                Debug::LogValue("cube pos z", t->GetPosition().z);
            }

            m = Parent->TryGetComponent<MeshFilter>();
            if (m)
            {
                Debug::LogValue("cube Type", m->GetType());
            }
        }
    }
};