#include "Citizen.h"
#include <Render/Renderer.h>
#include <Level/GameLevel.h>
#include <cmath>

using namespace Craft;
Citizen::Citizen(const Craft::Vector2& position)
    : Actor(L"C", position,Color::White)
{
    sortingOrder = 1;
    positionX = static_cast<float>(position.x);
    positionY = static_cast<float>(position.y);
}

void Citizen::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    if (path.empty())
    {
        return;
    }

    if (currentPathIndex >= path.size())
    {
        return;
    }

    Vector2 nextPosition = path[currentPathIndex];

    if (gameLevel->IsCitizenAt(nextPosition, this))
    {
        return;
    }

    float dx = static_cast<float>(nextPosition.x) - positionX;
    float dy = static_cast<float>(nextPosition.y) - positionY;

    float distance = std::sqrt(dx * dx + dy * dy);

    // 이미 목표 위치에 도착한 경우
    if (distance <= 0.0001f)
    {
        positionX = static_cast<float>(nextPosition.x);
        positionY = static_cast<float>(nextPosition.y);

        SetPosition(nextPosition);

        ++currentPathIndex;
        return;
    }

    float moveDistance = moveSpeed * deltaTime;

    // 이번 프레임에 목표 위치까지 도착할 수 있는 경우
    if (moveDistance >= distance)
    {
        positionX = static_cast<float>(nextPosition.x);
        positionY = static_cast<float>(nextPosition.y);

        SetPosition(nextPosition);

        if (gameLevel != nullptr)
        {
            gameLevel->UpdateQuadTree(this);
        }

        ++currentPathIndex;
        return;
    }

    // 목표 방향으로 이동
    float directionX = dx / distance;
    float directionY = dy / distance;

    positionX += directionX * moveDistance;
    positionY += directionY * moveDistance;

    SetPosition(
        Craft::Vector2(
            static_cast<int>(positionX),
            static_cast<int>(positionY)
        )
    );

    if (gameLevel != nullptr)
    {
        gameLevel->UpdateQuadTree(this);
    }
}

void Citizen::OnCollision(
    const std::shared_ptr<Craft::Actor>& other)
{
    super::OnCollision(other);
}

void Citizen::DebugDraw()
{
    for (int i = currentPathIndex; i < path.size(); ++i)
    {
        Renderer::Get().Submit(
            L".",
            path[i],
            Color::BrightWhite,
            0
        );
    }
}

void Citizen::SetTarget(
    const Craft::Vector2& target,
    const std::vector<std::vector<int>>& grid)
{
    this->target = target;

    path = astar.FindPath(
        GetPosition(),
        target,
        grid
    );

    currentPathIndex = 1;
}

void Citizen::SetGameLevel(GameLevel* level)
{
    gameLevel = level;
}

bool Citizen::HasPath() const
{
    return !path.empty() && currentPathIndex < path.size();
}

void Citizen::ClearPath()
{
    path.clear();
    currentPathIndex = 0;
}

void Citizen::SetTargetActor(const std::shared_ptr<Gimmick_Exit>&actor)
{
    targetExitGimmick = actor;
}

bool Citizen::HasTargetGimmick_Exit() const
{
    auto gimmick_exit = targetExitGimmick.lock();

    if (!gimmick_exit)
    {
        return false;
    }

    return !targetExitGimmick.expired();
}
