#pragma once
#include <Actor/Actor.h>
#include <Algorithm/Astar.h>
#include <vector>

class GameLevel;
class Gimmick_Exit;
class Citizen : public Craft::Actor
{
	TYPE_DECLARATIONS(Citizen, Actor);

public:
	Citizen(const Craft::Vector2& position);

	void SetTarget(const Craft::Vector2& target,
		const std::vector<std::vector<int>>& grid);
	void SetGameLevel(GameLevel* level);
	bool HasPath() const;
	void ClearPath();
	void SetTargetActor(const std::shared_ptr<Gimmick_Exit>& actor);
	bool HasTargetGimmick_Exit() const;
private:
	// 이벤트 함수 오버라이드.
	virtual void Tick(float deltaTime) override;

	// 충돌 이벤트 함수 오버라이드.
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;
	virtual void DebugDraw();

private:
	Craft::Astar astar;
	float moveSpeed = 2.0f;
	int currentPathIndex = 1;
	std::vector<Craft::Vector2> path;
	Craft::Vector2 target;
	GameLevel* gameLevel = nullptr;
	std::weak_ptr<Gimmick_Exit> targetExitGimmick;
	float positionX = 0.0f;
	float positionY = 0.0f;

	float debugElapsedTime = 0.0f;
	bool showDebugPath = false;
};

