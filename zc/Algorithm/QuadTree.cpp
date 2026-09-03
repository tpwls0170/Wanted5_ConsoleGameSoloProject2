#include "QuadTree.h"
using namespace Craft;

Craft::QuadTree::QuadTree(const Rect& bounds)
{
	root = std::make_unique<Node>();
	root->Bounds = bounds;
}
