    #pragma once

#include "../ECS/ECS.h"
#include "../Engine/Components.h"
#include <vector>

struct TrailPoint
{
    float x;
    float y;
    float alpha;
};

class TrailComponent : public Component
{
public:

    void init() override;
    void update() override;
    void draw() override;

private:

    TransformComponent* transform = nullptr;

    std::vector<TrailPoint> points;

    float timer = 0.0f;

    const float spawnInterval = 0.025f;
    const int maxPoints = 8;
};