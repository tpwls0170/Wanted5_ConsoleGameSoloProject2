#include "QuadTree.h"
using namespace Craft;

Craft::QuadTree::QuadTree(const Rect& bounds)
{
	root = std::make_unique<Node>();
	root->Bounds = bounds;
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
