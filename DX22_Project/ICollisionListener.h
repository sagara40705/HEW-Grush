#pragma once
class GameObject;

struct ICollisionListener
{
    virtual ~ICollisionListener() {}
    virtual void OnCollisionEnter(GameObject& other) = 0;
};
