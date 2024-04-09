#include "Wall.h"
#include "core/Draw.h"
#include "imgui.h"

Wall::Wall(vec2 sp, vec2 ep)
{
    startPoint = sp;
    endPoint = ep;
    middlePoint = vec2((sp.x + ep.x) / 2, (sp.y + ep.y) / 2);

    float dx = endPoint.x - startPoint.x;
    float dy = endPoint.y - startPoint.y;

    normal = glm::normalize(vec2(dy, -dx));
}

void Wall::Draw()
{
    Draw::Line(startPoint, endPoint);
}