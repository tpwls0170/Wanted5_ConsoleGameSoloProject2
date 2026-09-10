#include "PoliceActor.h"
#include <Level/GameLevel.h>
#include <Actor/PoliceBullet.h>
#include <Actor/Enemy/Zombie.h>
#include <Render/Renderer.h>
#include <cmath>
using namespace Craft;
PoliceActor::PoliceActor(const Craft::Vector2& position)
	:Actor(L"P",position,Craft::Color::Blue)
{
    positionX = static_cast<float>(position.x);
    positionY = static_cast<float>(position.y);
}

void PoliceActor::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    // 디버그 경로 표시
    debugElapsedTime += deltaTime;
    shootElapsedTime += deltaTime;
    if (debugElapsedTime >= 5.0f)
    {
        debugElapsedTime = 0.0f;
        showDebugPath = !showDebugPath;
    }

    if (showDebugPath && gameLevel->debugMode)
    {
        DebugDraw();
    }

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

    // 공격 사거리
    if (GetDistance(zombie->GetPosition()) <= range)
    {
        if (shootElapsedTime >= shootCooldown)
        {
            gameLevel->SpawnActor<PoliceBullet>(
                GetPosition(),
                zombie->GetPosition());

            shootElapsedTime = 0.0f;
            ClearPath();
        }
        return;
    }

    Vector2 nextPosition = path[currentPathIndex];

    // 현재 위치 → 다음 경로 지점
    float dx =
        static_cast<float>(nextPosition.x) - positionX;

    float dy =
        static_cast<float>(nextPosition.y) - positionY;

    float distance =
        std::sqrt(dx * dx + dy * dy);

    // 이미 목표 지점에 도착한 경우
    if (distance <= 0.0001f)
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

    float moveDistance = moveSpeed * deltaTime;

    // 이번 프레임에 목표까지 도착
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
        Craft::Vector2{
            static_cast<int>(positionX),
            static_cast<int>(positionY)
        }
    );

    if (gameLevel != nullptr)
    {
        gameLevel->UpdateQuadTree(this);
    }
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
