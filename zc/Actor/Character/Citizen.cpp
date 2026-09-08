#include "Citizen.h"
#include <Render/Renderer.h>
#include <Level/GameLevel.h>

using namespace Craft;
Citizen::Citizen(const Craft::Vector2& position)
    : Actor(L"C", position,Color::White)
{
    sortingOrder = 1;
}

void Citizen::Tick(float deltaTime)
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
    if (gameLevel->IsCitizenAt(nextPosition, this))
    {
        return;
    }
    
    SetPosition(nextPosition);

    if (gameLevel != nullptr)
    {
        gameLevel->UpdateQuadTree(this);
    }

    ++currentPathIndex;
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
