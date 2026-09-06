#include "Gimmick_Exit.h"
#include <Actor/Character/Citizen.h>

using namespace Craft;
Gimmick_Exit::Gimmick_Exit(const Craft::Vector2& position)
	:Actor(L"E",position,Craft::Color::Purple)
{
	sortingOrder = 5;
}

void Gimmick_Exit::OnCollision(const std::shared_ptr<Actor>&other)
{
	if (other->IsTypeOf<Citizen>())
	{
		other->Destroy();
	}
}
