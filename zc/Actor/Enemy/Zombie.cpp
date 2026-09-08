#include "Zombie.h"
#include <Actor/Character/Citizen.h>
#include <Actor/Character/PoliceActor.h>
#include <Level/GameLevel.h>
#include <Render/Renderer.h>
#include <cmath>

using namespace Craft;
Zombie::Zombie(const Craft::Vector2& position)
	: Actor(L"Z",position,Craft::Color::Red)
{
    sortingOrder = 1;
}

void Zombie::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    elapsedTime += deltaTime;

    debugElapsedTime += deltaTime;
    if (debugElapsedTime >= 5.0f)
    {
        debugElapsedTime = 0.0f;
        showDebugPath = !showDebugPath;
    }

    if (showDebugPath)
    {
        DebugDraw();
    }

    if (elapsedTime < 1.0f)
        return;

    elapsedTime = 0;
    if (path.empty())
    {
        return;
    }

    if (currentPathIndex >= path.size())
    {
        return;
    }

    Vector2 nextPosition = path[currentPathIndex];

    Vector2 crowdDirection = CalculateCrowdDirection();

    nextPosition.x += crowdDirection.x;
    nextPosition.y += crowdDirection.y;

    SetPosition(nextPosition);

    if (gameLevel != nullptr)
    {
        gameLevel->UpdateQuadTree(this);
    }

    ++currentPathIndex;
}

void Zombie::SetTarget(const Craft::Vector2& target, const std::vector<std::vector<int>>&grid)
{
    this->target = target;
    std::vector<std::vector<int>> zombieGrid = grid;

    for (auto& row : zombieGrid)
    {
        for (int& cell : row)
        {
            if (cell == 2)
            {
                cell = 3;
            }
        }
    }

    path = astar.FindPath(
        GetPosition(),
        target,
        zombieGrid
    );

    currentPathIndex = 0;
}

void Zombie::OnCollision(const std::shared_ptr<Actor>&other)
{
    if (other == nullptr)
        return;

    if (other->IsTypeOf<Citizen>())
    {
        // 여기까지 들어오는지 확인
    }
    else if (other->IsTypeOf<PoliceActor>())
    {
        // 여기까지 들어오는지 확인
    }
    else
    {
        return;
    }

    if (gameLevel != nullptr)
    {
        gameLevel->CreateZombie(
            other->GetPosition(),
            true
        );
    }

    ClearPath();
    other->Destroy();
}

Craft::Vector2 Zombie::CalculateCrowdDirection()
{
    Vector2 separation{ 0, 0 };

    if (gameLevel == nullptr)
        return separation;

    auto nearbyActors =
        gameLevel->GetQuadTree().QueryRange(
            GetPosition(),
            100.0f
        );

    for (Actor* actor : nearbyActors)
    {
        if (actor == nullptr)
            continue;

        if (actor == this)
            continue;

        Zombie* zombie =
            dynamic_cast<Zombie*>(actor);

        if (zombie == nullptr)
            continue;

        Vector2 direction =
            GetPosition() - zombie->GetPosition();

        float dx = static_cast<float>(direction.x);
        float dy = static_cast<float>(direction.y);

        float distance =
            std::sqrt(dx * dx + dy * dy);

        if (distance <= 0.0f)
            continue;

        if (distance < 50.0f)
        {
            int pushX = 0;
            int pushY = 0;

            if (direction.x > 0)
                pushX = 1;
            else if (direction.x < 0)
                pushX = -1;

            if (direction.y > 0)
                pushY = 1;
            else if (direction.y < 0)
                pushY = -1;

            separation.x =
                separation.x + pushX;

            separation.y =
                separation.y + pushY;
        }
    }

    if (separation.x > 1)
        separation.x = 1;
    else if (separation.x < -1)
        separation.x = -1;

    if (separation.y > 1)
        separation.y = 1;
    else if (separation.y < -1)
        separation.y = -1;

    return separation;

}

Craft::Vector2 Zombie::CalculateAlignment()
{
    Vector2 separation{ 0, 0 };

    if (gameLevel == nullptr)
        return separation;

    auto nearbyActors =
        gameLevel->GetQuadTree().QueryRange(
            GetPosition(),
            100.0f
        );

    for (Actor* actor : nearbyActors)
    {
        if (actor == nullptr)
            continue;

        if (actor == this)
            continue;

        Zombie* zombie =
            dynamic_cast<Zombie*>(actor);

        if (zombie == nullptr)
            continue;

        Vector2 direction =
            GetPosition() - zombie->GetPosition();

        float dx =
            static_cast<float>(direction.x);

        float dy =
            static_cast<float>(direction.y);

        float distance =
            std::sqrt(dx * dx + dy * dy);

        if (distance <= 0.0f)
            continue;

        if (distance < 50.0f)
        {
            int pushX = 0;
            int pushY = 0;

            if (direction.x > 0)
                pushX = 1;
            else if (direction.x < 0)
                pushX = -1;

            if (direction.y > 0)
                pushY = 1;
            else if (direction.y < 0)
                pushY = -1;

            separation.x =
                separation.x + pushX;

            separation.y =
                separation.y + pushY;
        }
    }

    // Separation 제한
    if (separation.x > 1)
        separation.x = 1;
    else if (separation.x < -1)
        separation.x = -1;

    if (separation.y > 1)
        separation.y = 1;
    else if (separation.y < -1)
        separation.y = -1;

    // ----------------------------------
    // Alignment
    // ----------------------------------
    Vector2 alignment =
        CalculateAlignment();

    // ----------------------------------
    // Cohesion
    // ----------------------------------
    Vector2 cohesion =
        CalculateCohesion();

    // ----------------------------------
    // 최종 군중 방향
    // ----------------------------------
    Vector2 result{ 0, 0 };

    result.x =
        separation.x +
        alignment.x +
        cohesion.x;

    result.y =
        separation.y +
        alignment.y +
        cohesion.y;

    // 최종적으로 너무 큰 보정 방지
    if (result.x > 1)
        result.x = 1;
    else if (result.x < -1)
        result.x = -1;

    if (result.y > 1)
        result.y = 1;
    else if (result.y < -1)
        result.y = -1;

    return result;
}

Craft::Vector2 Zombie::CalculateCohesion()
{
    Vector2 cohesion{ 0, 0 };

    if (gameLevel == nullptr)
        return cohesion;

    auto nearbyActors =
        gameLevel->GetQuadTree().QueryRange(
            GetPosition(),
            100.0f
        );

    int totalX = 0;
    int totalY = 0;
    int count = 0;

    for (Actor* actor : nearbyActors)
    {
        if (actor == nullptr || actor == this)
            continue;

        Zombie* zombie =
            dynamic_cast<Zombie*>(actor);

        if (zombie == nullptr)
            continue;

        totalX += zombie->GetPosition().x;
        totalY += zombie->GetPosition().y;

        count++;
    }

    if (count == 0)
        return cohesion;

    int centerX = totalX / count;
    int centerY = totalY / count;

    Vector2 direction =
        Vector2(centerX, centerY) - GetPosition();

    if (direction.x > 0)
        cohesion.x = 1;
    else if (direction.x < 0)
        cohesion.x = -1;

    if (direction.y > 0)
        cohesion.y = 1;
    else if (direction.y < 0)
        cohesion.y = -1;

    return cohesion;
}

void Zombie::DebugDraw()
{
    for (int i = currentPathIndex; i < path.size(); ++i)
    {
        Renderer::Get().Submit(
            L".",
            path[i],
            Color::Red,
            0
        );
    }
}

bool Zombie::HasTargetActor() const
{
    return !targetActor.expired();
}

bool Zombie::HasPath() const
{
    return !path.empty() && currentPathIndex < path.size();
}

void Zombie::SetGameLevel(GameLevel* level)
{
    gameLevel = level;
}

void Zombie::ClearPath()
{
    path.clear();
    currentPathIndex = 0;
}

void Zombie::SetTargetActor(const std::shared_ptr<Actor>& actor)
{
    targetActor = actor;
}

const std::weak_ptr<Actor> Zombie::GetTargetActor()
{
    return targetActor;
}
