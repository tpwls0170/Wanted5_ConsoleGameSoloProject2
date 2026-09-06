#pragma once
#include <Actor/Actor.h>
#include <Algorithm/Astar.h>
#include <memory>

class GameLevel;
class Citizen;
class Zombie : public Craft::Actor
{
	TYPE_DECLARATIONS(Zombie, Actor);
public:
	Zombie(const Craft::Vector2& position);

	void SetTarget(const Craft::Vector2& target,
		const std::vector<std::vector<int>>& grid);
	bool HasPath() const;
	void SetGameLevel(GameLevel* level);
	void ClearPath();
	void SetTargetCitizen(const std::shared_ptr<Citizen>& citizen);
	bool HasTargetCitizen() const;
private:
	// 이벤트 함수 오버라이드.
	virtual void Tick(float deltaTime) override;

	// 충돌 이벤트 함수 오버라이드.
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;
private:
	Craft::Astar astar;
	float moveSpeed = 0.0f;
	float elapsedTime = 0.0f;
	int currentPathIndex = 1;
	std::vector<Craft::Vector2> path;
	Craft::Vector2 target;
	GameLevel* gameLevel = nullptr;
	std::weak_ptr<Citizen> targetCitizen;
};

