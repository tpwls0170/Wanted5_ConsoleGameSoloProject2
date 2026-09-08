#include "PoliceActor.h"
#include <Level/GameLevel.h>
#include <Actor/PoliceBullet.h>
#include <Actor/Enemy/Zombie.h>
#include <Render/Renderer.h>

using namespace Craft;
PoliceActor::PoliceActor(const Craft::Vector2& position)
	:Actor(L"P",position,Craft::Color::Blue)
{
}

void PoliceActor::Tick(float deltaTime)
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

    if (elapsedTime < 0.8f)
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

    auto zombie = targetZombie.lock();

    if (!zombie || zombie->HasExpired())
    {
        targetZombie.reset();
        ClearPath();
        return;
    }

    if (zombie && GetDistance(zombie->GetPosition()) <= range)
    {
        gameLevel->SpawnActor<PoliceBullet>(
            GetPosition(),
            zombie->GetPosition());
        ClearPath();
        return;
    }

    Vector2 nextPosition = path[currentPathIndex];

    SetPosition(nextPosition);

    if (gameLevel != nullptr)
    {
        gameLevel->UpdateQuadTree(this);
    }

    ++currentPathIndex;
}

int PoliceActor::GetDistance(const Craft::Vector2 target) const
{
    return std::abs(GetPosition().x - target.x) + std::abs(GetPosition().y - target.y);
}

void PoliceActor::DebugDraw()
{
    for (int i = currentPathIndex; i < path.size(); ++i)
    {
        Renderer::Get().Submit(
            L".",
            path[i],
            Color::Blue,
            0
        );
    }
}

void PoliceActor::SetGameLevel(GameLevel* level)
{
	gameLevel = level;
}

bool PoliceActor::HasPath() const
{
	return !path.empty() && currentPathIndex < path.size();
}

void PoliceActor::ClearPath()
{
	path.clear();
	currentPathIndex = 0;
}

void PoliceActor::SetTargetZombie(const std::shared_ptr<Zombie>& zombie)
{
	targetZombie = zombie;
}

bool PoliceActor::HasTargetZombie() const
{
    auto zombie = targetZombie.lock();

    if (!zombie)
    {
        return false;
    }

	return !targetZombie.expired();
}

void PoliceActor::SetTarget(const Craft::Vector2& target, const std::vector<std::vector<int>>& grid)
{
    this->target = target;

    path = astar.FindPath(
        GetPosition(),
        target,
        grid
    );

    currentPathIndex = 0;
}
