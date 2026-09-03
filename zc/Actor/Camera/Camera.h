#pragma once
#include <Actor/Actor.h>

class Camera : public Craft::Actor
{
	TYPE_DECLARATIONS(Camera, Actor);
public:
	Camera(Craft::Vector2& position);
private:
	virtual void Tick(float deltaTime) override;
	Craft::Vector2 KeyInput();
	void Move(Craft::Vector2& position, float deltaTime);
};