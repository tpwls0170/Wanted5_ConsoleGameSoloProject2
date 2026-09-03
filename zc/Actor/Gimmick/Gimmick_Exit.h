#pragma once
#include <Actor/Actor.h>

class Gimmick_Exit : public Craft::Actor
{
	TYPE_DECLARATIONS(Gimmick_Exit, Actor);
public:
	Gimmick_Exit(const Craft::Vector2& position);
private:
	// 충돌 이벤트 함수 오버라이드.
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;
};

