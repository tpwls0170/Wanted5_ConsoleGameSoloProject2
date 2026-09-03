#include "Camera.h"
#include <Input/Input.h>

using namespace Craft;
Camera::Camera(Craft::Vector2& position)
{}

void Camera::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

Craft::Vector2 Camera::KeyInput()
{
	Vector2 direction = Craft::Vector2::Zero;

	if (Input::Get().GetKey(VK_RIGHT))
	{
		direction.x += 1;
	}
	else if (Input::Get().GetKey(VK_LEFT))
	{
		direction.x -=1;
	}
	else if (Input::Get().GetKey(VK_UP))
	{
		direction.y -= 1;
	}
	else if (Input::Get().GetKey(VK_DOWN))
	{
		direction.y += 1;
	}

	return direction;
}

void Camera::Move(Craft::Vector2& position, float deltaTime)
{}
