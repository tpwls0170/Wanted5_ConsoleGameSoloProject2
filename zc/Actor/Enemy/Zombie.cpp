#include "Zombie.h"
#include <Actor/Character/Citizen.h>
#include <Level/GameLevel.h>

#include <iostream>
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

    path = astar.FindPath(
        GetPosition(),
        target,
        grid
    );

    currentPathIndex = 0;
}

void Zombie::OnCollision(const std::shared_ptr<Actor>&other)
{
    if (other->IsTypeOf<Citizen>())
    {
        if (gameLevel != nullptr)
        {
            gameLevel->CreateZombie(other->GetPosition());
        }

        ClearPath();
        other->Destroy();
    }
}

bool Zombie::HasTargetCitizen() const
{
    return !targetCitizen.expired();
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

void Zombie::SetTargetCitizen(const std::shared_ptr<Citizen>& citizen)
{
    targetCitizen = citizen;
}
