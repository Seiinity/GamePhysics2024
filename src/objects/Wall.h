#pragma once

#include <glm/glm.hpp>
using glm::vec2;

class Wall
{
private:

    vec2 startPoint;
    vec2 endPoint;
    vec2 middlePoint;
    vec2 normal;

public:

	Wall(vec2 sp = vec2(0, 0), vec2 ep = vec2(0, 0));

	void Draw();

    vec2 GetStartPoint() { return startPoint; }
    vec2 GetEndPoint() { return endPoint; }
    vec2 GetNormal() { return normal; }
};