#pragma once
#include <Math/Vector2.h>

namespace Craft
{
    class Astar
    {
    public:
        std::vector<Vector2> FindPath(
            const Vector2& start,
            const Vector2& target,
            const std::vector<std::vector<int>>& grid);

        struct DebugNode
        {
            Vector2 position;
            int gCost;
            int hCost;
            int fCost;
        };
        
        const std::vector<DebugNode>& GetDebugNodes() const;
    private:
        struct Node
        {
            Vector2 position;

            Node* parent = nullptr;

            int gCost = 0;
            int hCost = 0;
            int fCost = 0;
        };
        int GetDistance(const Vector2& a, const Vector2& b);

        std::vector<DebugNode> debugNode;
    };
}
