#include "PoliceBullet.h"
#include <Actor/Enemy/Zombie.h>
#include <cmath>

using namespace Craft;
PoliceBullet::PoliceBullet(const Craft::Vector2& position,
	const Craft::Vector2& targetPosition)
	:Actor(L"*",position,Craft::Color::BrightWhite)
{
	positionX = static_cast<float>(position.x);
	positionY = static_cast<float>(position.y);

	float dx = static_cast<float>(targetPosition.x - position.x);
	float dy = static_cast<float>(targetPosition.y - position.y);

	float distance = std::sqrt(dx * dx + dy * dy);

	directionX = dx / distance;
	directionY = dy / distance;
}

void PoliceBullet::Tick(float deltaTime)
{
	elapsedTime += deltaTime;

	if (elapsedTime >= 2.5f)
	{
		this->Destroy();
		elapsedTime = 0;
		return;
	}
	positionX += directionX * moveSpeed * deltaTime;
	positionY += directionY * moveSpeed * deltaTime;

	SetPosition(Vector2{ static_cast<int>(positionX),static_cast<int>(positionY) });
}

void PoliceBullet::OnCollision(const std::shared_ptr<Actor>&other)
{
	if (other->IsTypeOf<Zombie>())
	{
		other->Destroy();
		this->Destroy();
	}
}
