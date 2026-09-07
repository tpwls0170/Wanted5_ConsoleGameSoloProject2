#include "Zombie.h"
#include <Actor/Character/Citizen.h>
#include <Actor/Character/PoliceActor.h>
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
    if (other->IsTypeOf<Citizen>())
    {
        if (gameLevel != nullptr)
        {
            gameLevel->CreateZombie(other->GetPosition(), true);
        }
    }
    else if (other->IsTypeOf<PoliceActor>())
    {
        if (gameLevel != nullptr)
        {
            gameLevel->CreateZombie(other->GetPosition(), true);
        }
    }

    ClearPath();
    other->Destroy();
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
