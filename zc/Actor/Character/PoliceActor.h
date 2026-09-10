#pragma once
#include <Actor/Actor.h>
#include <Algorithm/Astar.h>

class GameLevel;
class Zombie;
class PoliceActor : public Craft::Actor
{
	TYPE_DECLARATIONS(PoliceActor, Actor);
public:
	PoliceActor(const Craft::Vector2& position);
	void SetTarget(const Craft::Vector2& target,
		const std::vector<std::vector<int>>& grid);
	void SetGameLevel(GameLevel* level);
	bool HasPath() const;
	void ClearPath();
	void SetTargetZombie(const std::shared_ptr<Zombie>& zombie);
	bool HasTargetZombie() const;
private:
	virtual void Tick(float deltaTime) override;
	int GetDistance(const Craft::Vector2 target) const;
	void DebugDraw();
private:
	Craft::Astar astar;
	float moveSpeed = 2.0f;
	int currentPathIndex = 1;
	int range = 5;
	std::vector<Craft::Vector2> path;
	Craft::Vector2 target;
	GameLevel* gameLevel = nullptr;
	std::weak_ptr<Zombie> targetZombie;
	float shootCooldown = 2.0f;
	float shootElapsedTime = 0.0f;
	float positionX = 0.0f;
	float positionY = 0.0f;

	float debugElapsedTime = 0.0f;
	bool showDebugPath = false;
};

