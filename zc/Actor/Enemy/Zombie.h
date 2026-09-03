#pragma once
#include <Actor/Actor.h>

class Zombie : public Craft::Actor
{
	TYPE_DECLARATIONS(Zombie, Actor);
public:
	Zombie(const Craft::Vector2& position);
private:

};

