#include "Zombie.h"

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

    std::cout << "Zombie Move : "
        << path[currentPathIndex].x << ", "
        << path[currentPathIndex].y << std::endl;

    Vector2 nextPosition = path[currentPathIndex];

    SetPosition(nextPosition);

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

}

bool Zombie::HasPath() const
{
    return !path.empty() && currentPathIndex < path.size();
}
