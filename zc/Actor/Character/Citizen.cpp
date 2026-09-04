#include "Citizen.h"
#include <Render/Renderer.h>

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

    SetPosition(path[currentPathIndex]);

    ++currentPathIndex;
}

void Citizen::OnCollision(
    const std::shared_ptr<Craft::Actor>& other)
{
    super::OnCollision(other);
}

void Citizen::Draw()
{
    super::Draw();
    //a*경로 디버그 모드 나중에 수정하기
  /* const auto& debugNodes = astar.GetDebugNodes();

    for (const auto& node : debugNodes)
    {
        Renderer::Get().Submit(
            L".",
            node.position,
            Color::Green,
            0
        );
    }*/
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

    currentPathIndex = 0;
}