#include "QuadTree.h"
#include <algorithm>

using namespace Craft;

Craft::QuadTree::QuadTree(const Rect& bounds)
{
	root = std::make_unique<Node>();
	root->Bounds = bounds;
}

bool Craft::QuadTree::Update(Actor* actor)
{
	return root->Update(actor);
}

bool Craft::QuadTree::Insert(Actor* actor)
{
	return root->Insert(actor);
}

std::vector<Actor*> Craft::QuadTree::Query(const Rect& area)
{
	std::vector<Actor*> result;

	root->Query(area, result);
	return result;
}

bool Craft::QuadTree::Remove(Actor* actor)
{
	return root->Remove(actor);
}

std::vector<DebugRect> Craft::QuadTree::DebugDraw()
{
	std::vector<DebugRect> debugRects;
	root->GetDebugRects(debugRects);
	return debugRects;
}

std::vector<Actor*> Craft::QuadTree::QueryRange(const Vector2& center, float range)
{
	Rect area;

	area.x = center.x - range;
	area.y = center.y - range;
	area.width = range * 2.0f;
	area.height = range * 2.0f;

	return Query(area);
}

