#include "Gimmick_Exit.h"

using namespace Craft;
Gimmick_Exit::Gimmick_Exit(const Craft::Vector2& position)
	:Actor(L"🚪",position,Color::Yellow)
{
	sortingOrder = 5;
}

void Gimmick_Exit::OnCollision(const std::shared_ptr<Actor>&other)
{

}
