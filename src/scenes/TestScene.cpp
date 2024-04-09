#include "TestScene.h"
#include "core/Draw.h"
#include "imgui.h"
#include "iostream"
#include "core/Colors.h"

using glm::vec2;

TestScene::TestScene() : spawnForceX(0), spawnForceY(0), useRandomRadius(false), circleSpawnRadius(.5), gravity(0, 9.8)
{
    circles = 
    {
        new Circle(.8, vec2(2, 8), vec2(3, 0), Colors::red),
        new Circle(.5, vec2(-1, 7), vec2(2, 0), Colors::green),
    };

    walls = 
    {
        new Wall(vec2(-4, 0), vec2(1, 0)),
        new Wall(vec2(1, 0), vec2(5, 4)),
        new Wall(vec2(5, 7), vec2(5, 4)),
        new Wall(vec2(-4, 3), vec2(-4, 0)),
        new Wall(vec2(-4, 3), vec2(-5, 4)),
        new Wall(vec2(-5, 4), vec2(-10, 4)),
        new Wall(vec2(-10, 4), vec2(-10, 10)),
        new Wall(vec2(-3, 10), vec2(5, 7)),
        new Wall(vec2(-3, 10), vec2(-10, 10))
    };
}

TestScene::~TestScene()
{
    for (Circle* circle : circles) delete circle;
    for (Wall* wall : walls) delete wall;
}

void TestScene::OnEnable() {}
void TestScene::OnDisable() {}

void TestScene::Update(float deltaTime)
{
    for (Circle* circle : circles)
    {
        UpdateCircleOnWallCollisions(circle);
        UpdateCircleOnCircleCollisions(circle);

        circle->UpdatePosition(gravity, deltaTime);
    }
}

void TestScene::Draw()
{ 
    for (Wall* wall : walls) wall->Draw();
    for (Circle* circle : circles) circle->Draw();
}

void TestScene::DrawGUI()
{
    ImGui::Begin("Inspector");

    ImGui::SeparatorText("Circle Radius");
    ImGui::SliderFloat("Radius", &circleSpawnRadius, .3f, .8f);
    ImGui::Checkbox("Random Radius", &useRandomRadius);

    ImGui::SeparatorText("Spawn Force");
    ImGui::SliderFloat("Force X", &spawnForceX, -20, 20);
    ImGui::SliderFloat("Force Y", &spawnForceY, -20, 20);
    
    ImGui::SeparatorText("Commands");

    if (ImGui::Button("Spawn Ball"))
    {
        double circleRadius = useRandomRadius ? (rand() / (double)RAND_MAX) * (.7 - .3) + .3 : circleSpawnRadius;
        ImColor circleColour = ImColor::HSV(rand() / (double)RAND_MAX, rand() / (double)RAND_MAX * .5 + .5, 1);
        vec2 circleForce = vec2(spawnForceX, spawnForceY);

        circles.push_back(new Circle(circleRadius, vec2(0, 5), circleForce, circleColour));
    }

    if (ImGui::Button("Despawn Ball") && circles.size() > 0)
    {
       delete circles[circles.size() - 1];
       circles.pop_back();
    };

    ImGui::End();
}

// Updates a circle's collision with other circles.
void TestScene::UpdateCircleOnCircleCollisions(Circle* circle)
{
    for (Circle* collisionCircle : circles)
    {
        if (collisionCircle == circle) continue; // Ignores collision with self.
        if (!circle->CheckCollisionWithCircle(collisionCircle)) continue;

        circle->HandleCollisionWithCircle(collisionCircle);
    }
}

// Updates a circle's collision with walls.
void TestScene::UpdateCircleOnWallCollisions(Circle* circle)
{
    for (Wall* wall : walls)
    {
        if (circle->CheckCollisionWithWall(wall)) circle->HandleCollisionWithWall(wall);
    }
}