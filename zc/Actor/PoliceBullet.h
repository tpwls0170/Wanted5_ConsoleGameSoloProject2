#pragma once
#include <Actor/Actor.h>

class PoliceBullet : public Craft::Actor
{
	TYPE_DECLARATIONS(PoliceBullet, Actor);
public:
	PoliceBullet(const Craft::Vector2& position, const Craft::Vector2& targetPosition);
private:
	virtual void Tick(float deltaTime) override;
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

private:
	float moveSpeed = 5.0f;
	float elapsedTime = 0.0f;
	float directionX = 0.0f;
	float directionY = 0.0f;
	float positionX = 0.0f;
	float positionY = 0.0f;
};

