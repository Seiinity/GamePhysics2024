#pragma once

#include "core/Simple2DScene.h"
#include <glm/glm.hpp>
#include <vector>
#include <objects/Circle.h>
#include <objects/Wall.h>

class TestScene : public Simple2DScene
{
private:

    glm::vec2 gravity;

    std::vector<Circle*> circles;
    std::vector<Wall*> walls;

    float circleSpawnRadius;
    bool useRandomRadius;

    float spawnForceX;
    float spawnForceY;

    void UpdateCircleOnCircleCollisions(Circle* circle);
    void UpdateCircleOnWallCollisions(Circle* circle);

public:

    TestScene();
    ~TestScene();

    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void DrawGUI() override;

    virtual const char* GetName() override { return "Test Scene"; }
};