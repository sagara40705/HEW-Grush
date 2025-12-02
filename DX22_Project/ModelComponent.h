#pragma once
#include "Component.h"
#include "Model.h"
#include <memory>
#include <string>

class ModelComponent : public Component
{
public:
    //std::shared_ptr<Model> model{};
    //std::string path{};
    //float scale = 1.0f;
    //Model::Flip flip = Model::Flip::None;
    Model* model = nullptr;
    float boundsRadius = 22.0f;

public:
    ModelComponent() = default;
    //ModelComponent(const char* file, float s = 1.0f, Model::Flip f = Model::Flip::None) : path(file ? file : ""), scale(s), flip(f) {}
    ModelComponent(Model* m) { model = m; }
    ~ModelComponent() override = default;

    void Start() override { /*LoadIfNeeded();*/  }
    void OnDestroy() override { /*model.reset(); */ model = nullptr; }

    void SetSource(const std::string& file, float s = 1.0f, Model::Flip f = Model::Flip::None)
    {
        //path = file; scale = s; flip = f;
        //LoadIfNeeded(true);
    }

    Model* Get() const { return model; }

private:
    void LoadIfNeeded(bool force = false)
    {
        //if (!force && model) return;
        //if (path.empty())  return;

        //model = std::make_shared<Model>();
        //if (!model->Load(path.c_str(), scale, flip))
        //{
        //    model.reset(); // Ž¸”sŽž‚Í‹ó‚É–ß‚·
        //    assert(model && "model::load model = nullptr");
        //}
    }
};
