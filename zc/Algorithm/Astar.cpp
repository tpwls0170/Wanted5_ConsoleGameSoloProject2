#include "Astar.h"

using namespace Craft;

int Astar::GetDistance(const Vector2& a, const Vector2& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<Vector2> Astar::FindPath(
    const Vector2& start,
    const Vector2& target,
    const std::vector<std::vector<int>>& grid)
{
    debugNode.clear();
    std::vector<Vector2> path;

    if (grid.empty())
    {
        return path;
    }

    if (start == target)
    {
        path.push_back(start);
        return path;
    }

    const int height = static_cast<int>(grid.size());
    const int width = static_cast<int>(grid[0].size());

    // Open List
    std::vector<Node*> openList;

    // Closed List
    std::vector<Node*> closedList;

    Node* startNode = new Node();
    startNode->position = start;
    startNode->gCost = 0;
    startNode->hCost = GetDistance(start, target);
    startNode->fCost = startNode->gCost + startNode->hCost;

    openList.push_back(startNode);

    // 상, 하, 좌, 우
    const Vector2 directions[4] =
    {
        { 0, -1 },
        { 0,  1 },
        {-1,  0 },
        { 1,  0 }
    };

    while (!openList.empty())
    {
        // 가장 작은 F Cost를 가진 Node 찾기
        Node* currentNode = openList[0];

        for (Node* node : openList)
        {
            if (node->fCost < currentNode->fCost ||
                (node->fCost == currentNode->fCost &&
                    node->hCost < currentNode->hCost))
            {
                currentNode = node;
            }
        }

        // Open -> Closed
        openList.erase(
            std::remove(openList.begin(), openList.end(), currentNode),
            openList.end());

        closedList.push_back(currentNode);

        // 목적지 도착
        if (currentNode->position == target)
        {
            Node* node = currentNode;

            while (node != nullptr)
            {
                path.push_back(node->position);
                node = node->parent;
            }

            // 목적지 -> 시작점으로 들어왔기 때문에 뒤집기
            std::reverse(path.begin(), path.end());

            break;
        }

        // 주변 4방향 탐색
        for (const Vector2& direction : directions)
        {
            Vector2 nextPosition =
            {
                currentNode->position.x + direction.x,
                currentNode->position.y + direction.y
            };

            // 맵 범위 검사
            if (nextPosition.x < 0 ||
                nextPosition.x >= width ||
                nextPosition.y < 0 ||
                nextPosition.y >= height)
            {
                continue;
            }

            // 이미 탐색한 위치인지 확인
            bool isClosed = false;

            for (Node* node : closedList)
            {
                if (node->position == nextPosition)
                {
                    isClosed = true;
                    break;
                }
            }

            if (isClosed)
            {
                continue;
            }

            // 0 = 이동 가능
            // 1 = 시민
            // 2 = 탈출구
            //
            // 탈출구는 이동 가능해야 하므로
            // 0 또는 2만 통과
            if (grid[nextPosition.y][nextPosition.x] == 1)
            {
                continue;
            }

            int newGCost = currentNode->gCost + 1;

            Node* nextNode = nullptr;

            for (Node* node : openList)
            {
                if (node->position == nextPosition)
                {
                    nextNode = node;
                    break;
                }
            }

            if (nextNode == nullptr)
            {
                nextNode = new Node();

                nextNode->position = nextPosition;
                nextNode->parent = currentNode;
                nextNode->gCost = newGCost;
                nextNode->hCost = GetDistance(nextPosition, target);
                nextNode->fCost =
                    nextNode->gCost + nextNode->hCost;

                openList.push_back(nextNode);

                debugNode.push_back({
                nextNode->position,
                nextNode->gCost,
                nextNode->hCost,
                nextNode->fCost
                });
            }
            else if (newGCost < nextNode->gCost)
            {
                nextNode->parent = currentNode;
                nextNode->gCost = newGCost;
                nextNode->fCost =
                    nextNode->gCost + nextNode->hCost;
            }
        }
    }

    // 메모리 정리
    for (Node* node : openList)
    {
        delete node;
    }

    for (Node* node : closedList)
    {
        delete node;
    }

    return path;
}

const std::vector<Astar::DebugNode>& Astar::GetDebugNodes() const
{
    return debugNode;
}