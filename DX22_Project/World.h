#pragma once
#include <deque>
#include <vector>
#include <memory>
#include <algorithm>
#include "GameObject.h"

class World
{
public:
    std::vector<std::unique_ptr<GameObject>> objects;
    std::vector<ObjectId>  toDestroy{};
    ObjectId nextId= 1;
public:
    GameObject& CreateGameObject();
    void RequestDestroy(ObjectId id);
    // •Ö—˜ŠÖ”
    GameObject* Find(ObjectId id) noexcept;
    const GameObject* Find(ObjectId id) const noexcept;
public:
    void Start();
    void Update();
    void Draw();
    void ClearAll();
private:
    // © ’Ç‰ÁF”j‰óƒLƒ…[‚ğˆ—
    void ProcessDestroyQueue();
public:
    World() = default;
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    World(World&&) = delete;            // © ’Ç‰Á
    World& operator=(World&&) = delete; // © ’Ç‰Á
};

extern World W;