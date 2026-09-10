#include "Zombie.h"
#include <Actor/Character/Citizen.h>
#include <Actor/Character/PoliceActor.h>
#include <Level/GameLevel.h>
#include <Render/Renderer.h>
#include <cmath>

using namespace Craft;
Zombie::Zombie(const Craft::Vector2& position)
    : Actor(L"Z", position, Craft::Color::Red)
{
    sortingOrder = 1;
    positionX = static_cast<float>(position.x);
    positionY = static_cast<float>(position.y);
}

void Zombie::Tick(float deltaTime)
{
    auto target = targetActor.lock();

    if (target != nullptr && !target->HasExpired())
    {
        Vector2 zombiePosition = GetPosition();
        Vector2 targetPosition = target->GetPosition();

        int distance =
            std::abs(zombiePosition.x - targetPosition.x) +
            std::abs(zombiePosition.y - targetPosition.y);

        if (target->IsTypeOf<Citizen>() && distance <= 2)
        {
            OnCollision(target);
            return;
        }

        if (target->IsTypeOf<PoliceActor>() && distance <= 1)
        {
            OnCollision(target);
            return;
        }
    }

    // 디버그 경로 표시
    debugElapsedTime += deltaTime;

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
        return;

    if (currentPathIndex >= path.size())
        return;

    Vector2 nextPosition = path[currentPathIndex];

    // 현재 위치 → 다음 경로 지점
    float dx =
        static_cast<float>(nextPosition.x) - positionX;

    float dy =
        static_cast<float>(nextPosition.y) - positionY;

    // Boids
    CrowdVelocity boids =
        CalculateBoidsSteering();

    dx += boids.x * 0.1f;
    dy += boids.y * 0.1f;

    float distance =
        std::sqrt(dx * dx + dy * dy);

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

    // 목표 방향으로 조금씩 이동
    float directionX = dx / distance;
    float directionY = dy / distance;

    velocity.x = directionX;
    velocity.y = directionY;

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

void Zombie::SetTarget(const Craft::Vector2& target, const std::vector<std::vector<int>>& grid)
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

void Zombie::OnCollision(const std::shared_ptr<Actor>& other)
{
    if (other == nullptr)
        return;

    if (other->IsTypeOf<Citizen>())
    {
        if (gameLevel != nullptr)
        {
            gameLevel->CreateZombie(
                other->GetPosition(),
                true
            );
        }
    }
    else if (other->IsTypeOf<PoliceActor>())
    {
        if (gameLevel != nullptr)
        {
            gameLevel->CreateZombie(
                other->GetPosition(),
                true
            );
        }
    }
    else
    {
        return;
    }

    ClearPath();
    ClearTargetActor();
    SetSurroundSlot(-1);

    other->Destroy();
}

Zombie::CrowdVelocity Zombie::CalculateSeparation()
{
    CrowdVelocity separation{};

    if (gameLevel == nullptr)
        return separation;

    auto nearbyActors =
        gameLevel->GetQuadTree().QueryRange(
            GetPosition(),
            3.0f
        );

    for (Actor* actor : nearbyActors)
    {
        if (actor == nullptr || actor == this)
            continue;

        Zombie* zombie =
            dynamic_cast<Zombie*>(actor);

        if (zombie == nullptr)
            continue;

        float dx =
            positionX -
            static_cast<float>(zombie->GetPosition().x);

        float dy =
            positionY -
            static_cast<float>(zombie->GetPosition().y);

        float distance =
            std::sqrt(dx * dx + dy * dy);

        if (distance <= 0.0001f)
            continue;

        const float separationRadius = 2.0f;

        if (distance < separationRadius)
        {
            float strength =
                (separationRadius - distance) /
                separationRadius;

            separation.x +=
                (dx / distance) * strength;

            separation.y +=
                (dy / distance) * strength;
        }
    }

    return separation;
}

Zombie::CrowdVelocity Zombie::CalculateAlignment()
{
    CrowdVelocity alignment{ 0.0f, 0.0f };

    if (gameLevel == nullptr)
        return alignment;

    auto nearbyActors =
        gameLevel->GetQuadTree().QueryRange(
            GetPosition(),
            5.0f
        );

    int neighborCount = 0;

    for (Actor* actor : nearbyActors)
    {
        if (actor == nullptr || actor == this)
            continue;

        Zombie* zombie =
            dynamic_cast<Zombie*>(actor);

        if (zombie == nullptr)
            continue;

        alignment.x += zombie->GetVelocity().x;
        alignment.y += zombie->GetVelocity().y;

        ++neighborCount;
    }

    if (neighborCount == 0)
        return alignment;

    alignment.x /= neighborCount;
    alignment.y /= neighborCount;

    float length =
        std::sqrt(
            alignment.x * alignment.x +
            alignment.y * alignment.y
        );

    if (length > 0.0f)
    {
        alignment.x /= length;
        alignment.y /= length;
    }

    return alignment;
}

Zombie::CrowdVelocity Zombie::CalculateCohesion()
{
    CrowdVelocity cohesion{ 0, 0 };

    if (gameLevel == nullptr)
        return cohesion;

    auto nearbyActors =
        gameLevel->GetQuadTree().QueryRange(
            GetPosition(),
            5.0f
        );

    int neighborCount = 0;

    float averageX = 0.0f;
    float averageY = 0.0f;

    for (Actor* actor : nearbyActors)
    {
        if (actor == nullptr || actor == this)
            continue;

        Zombie* zombie =
            dynamic_cast<Zombie*>(actor);

        if (zombie == nullptr)
            continue;

        averageX += static_cast<float>(
            zombie->GetPosition().x);

        averageY += static_cast<float>(
            zombie->GetPosition().y);

        ++neighborCount;
    }

    if (neighborCount == 0)
        return cohesion;

    averageX /= neighborCount;
    averageY /= neighborCount;

    cohesion.x =
        averageX - static_cast<float>(GetPosition().x);

    cohesion.y =
        averageY - static_cast<float>(GetPosition().y);

    float length =
        std::sqrt(
            cohesion.x * cohesion.x +
            cohesion.y * cohesion.y
        );

    if (length > 0.0f)
    {
        cohesion.x /= length;
        cohesion.y /= length;
    }

    return cohesion;
}

Zombie::CrowdVelocity Zombie::CalculateBoidsSteering()
{
    CrowdVelocity separation = CalculateSeparation();
    CrowdVelocity alignment = CalculateAlignment();
    CrowdVelocity cohesion = CalculateCohesion();

    CrowdVelocity steering{};

    steering.x =
        separation.x * separationWeight +
        alignment.x * alignmentWeight +
        cohesion.x * cohesionWeight;

    steering.y =
        separation.y * separationWeight +
        alignment.y * alignmentWeight +
        cohesion.y * cohesionWeight;

    float length =
        std::sqrt(
            steering.x * steering.x +
            steering.y * steering.y
        );

    if (length > 1.0f)
    {
        steering.x /= length;
        steering.y /= length;
    }

    return steering;
}

Zombie::CrowdVelocity Zombie::CalculatePathDirection()
{
    CrowdVelocity direction{};

    if (path.empty())
        return direction;

    if (currentPathIndex >= path.size())
        return direction;

    Vector2 nextPosition = path[currentPathIndex];

    float dx =
        static_cast<float>(nextPosition.x) - positionX;

    float dy =
        static_cast<float>(nextPosition.y) - positionY;

    float length =
        std::sqrt(dx * dx + dy * dy);

    if (length <= 0.0f)
        return direction;

    direction.x = dx / length;
    direction.y = dy / length;

    return direction;
}

Zombie::CrowdVelocity Zombie::CalculateFinalDirection()
{
    CrowdVelocity pathDirection = CalculatePathDirection();
    CrowdVelocity boidsDirection = CalculateBoidsSteering();

    CrowdVelocity finalDirection{};

    const float pathWeight = 1.0f;
    const float boidsWeight = 0.1f;

    finalDirection.x =
        pathDirection.x * pathWeight +
        boidsDirection.x * boidsWeight;

    finalDirection.y =
        pathDirection.y * pathWeight +
        boidsDirection.y * boidsWeight;

    float length =
        std::sqrt(
            finalDirection.x * finalDirection.x +
            finalDirection.y * finalDirection.y
        );

    if (length > 0.0f)
    {
        finalDirection.x /= length;
        finalDirection.y /= length;
    }

    return finalDirection;
}

void Zombie::ClearTargetActor()
{
    targetActor.reset();
}

void Zombie::SetSurroundSlot(int slot)
{
    surroundSlot = slot;
}

const Zombie::CrowdVelocity& Zombie::GetVelocity() const
{
    return velocity;
}

Craft::Vector2 Zombie::CalculateSurroundPosition()
{
    auto target = targetActor.lock();

    if (target == nullptr)
    {
        return GetPosition();
    }

    Vector2 targetPosition = target->GetPosition();

    const Vector2 positions[8] =
    {
        { -1, -1 }, // 0
        {  0, -1 }, // 1
        {  1, -1 }, // 2
        { -1,  0 }, // 3
        {  1,  0 }, // 4
        { -1,  1 }, // 5
        {  0,  1 }, // 6
        {  1,  1 }  // 7
    };

    if (surroundSlot < 0 || surroundSlot >= 8)
    {
        return GetPosition();
    }

    return targetPosition + positions[surroundSlot];
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
