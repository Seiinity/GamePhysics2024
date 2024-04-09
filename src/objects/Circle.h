#pragma once

#include <glm/glm.hpp>
#include <core/Colors.h>
#include "Wall.h"

using glm::vec2;

class Circle
{
private:

    float radius;
    vec2 position;
    vec2 velocity;
    ImColor colour;

    /* Collision Detection */

    vec2 GetBounceDirectionFromCircle(Circle* circle);
    vec2 GetBounceDirectionFromWall(Wall* wall);
    vec2 GetClosestPointOnWall(Wall* wall);

public:

    Circle(float r = 1, vec2 p = vec2(0, 0), vec2 v = vec2(0, 0), ImColor c = Colors::green);

    void UpdatePosition(vec2 gravity, float deltaTime);
    void Draw();

    /* Collision Detection */

    bool CheckCollisionWithCircle(Circle* circle);
    bool CheckCollisionWithWall(Wall* wall);

    void HandleCollisionWithWall(Wall* wall);
    void HandleCollisionWithCircle(Circle* circle);
};