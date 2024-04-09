#include "Circle.h"
#include "core/Draw.h"
#include "imgui.h"
#include <random>

Circle::Circle(float r, vec2 p, vec2 v, ImColor c)
{
    radius = r;
    position = p;
    velocity = v;
    colour = c;
}

void Circle::Draw()
{
    Draw::SetColor(colour);
    Draw::Circle(position, radius, true);
}

void Circle::UpdatePosition(vec2 gravity, float deltaTime)
{
    velocity -= gravity * deltaTime;
    position += velocity * deltaTime;
}

#pragma region CollisionDetection

// Returns the direction the circle should bounce toward after colliding with another circle.
vec2 Circle::GetBounceDirectionFromCircle(Circle* circle)
{
    vec2 touchPoint = vec2((position.x + circle->position.x) / 2, (position.y + circle->position.y) / 2);
    vec2 normal = glm::normalize(vec2(position.x - touchPoint.x, position.y - touchPoint.y));

    vec2 u = (glm::dot(velocity, normal) / glm::dot(normal, normal)) * normal;
    vec2 w = velocity - normal;

    return velocity - 2.0f * dot(velocity, normal) * normal;
}

// Returns the direction the circle should bounce toward after colliding with a wall.
vec2 Circle::GetBounceDirectionFromWall(Wall* wall)
{
    vec2 normal = wall->GetNormal();

    vec2 u = (glm::dot(velocity, normal) / glm::dot(normal, normal)) * normal;
    vec2 w = velocity - normal;

    return velocity - 2.0f * glm::dot(velocity, normal) * normal;
}

// Gets the closest point to the circle on a wall.
vec2 Circle::GetClosestPointOnWall(Wall* wall)
{
    vec2 a = wall->GetStartPoint();
    vec2 b = wall->GetEndPoint();
    vec2 c = position;

    vec2 ab = b - a;
    vec2 ac = c - a;

    // Projects AC onto AB.
    float projectionLength = glm::dot(ac, ab) / glm::dot(ab, ab);

    if (projectionLength < 0.0f) return a; // Collided with the start of the wall.
    else if (projectionLength > 1.0f) return b; // Collided with the end of the wall.
    else return a + projectionLength * ab; // Collided with some other point of the wall.
}

// Checks if the circle is colliding with another circle.
bool Circle::CheckCollisionWithCircle(Circle* circle)
{
    float dx = position.x - circle->position.x;
    float dy = position.y - circle->position.y;
    float d = (dx * dx) + (dy * dy);

    float radii = radius + circle->radius;

    return radii * radii >= d;
}

// Checks if the circle is colliding with a wall.
bool Circle::CheckCollisionWithWall(Wall* wall)
{
    vec2 closestPoint = GetClosestPointOnWall(wall);

    vec2 distanceVector = position - closestPoint;
    float distance = glm::length(distanceVector);

    return distance <= radius;
}

// Handles the circle's collision with another circle.
void Circle::HandleCollisionWithCircle(Circle* circle)
{
    // Calculates how much to move the circle out of the other circle.
    vec2 d = position - circle->position;
    float distance = sqrt(d.x * d.x + d.y * d.y);
    float overlap = (radius + circle->radius) - distance;

    // Adjusts both circles' position if there's an overlap.
    if (overlap > 0) {
        vec2 direction(d.x / distance, d.y / distance);
        position += direction * (overlap / 2.0f +
                                 .05f); // Half the overlap for each + buffer.
        circle->position -= direction * (overlap / 2.0f + .05f);
    }

    velocity = GetBounceDirectionFromCircle(circle);
    circle->velocity = circle->GetBounceDirectionFromCircle(this);
}

// Handles the circle's collision with a wall.
void Circle::HandleCollisionWithWall(Wall* wall)
{
    // Calculates how much to move the circle out of the wall.
    vec2 distance = position - GetClosestPointOnWall(wall);
    float overlap = radius - glm::length(distance);

    // Adjusts the circle's position if there's an overlap.
    if (overlap > 0)
    {
        vec2 direction = glm::normalize(distance);
        position += direction * overlap;
    }

    velocity = GetBounceDirectionFromWall(wall);
}

#pragma endregion