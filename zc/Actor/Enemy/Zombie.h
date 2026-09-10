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
	struct CrowdVelocity
	{
		float x = 0.0f;
		float y = 0.0f;
	};

	Zombie(const Craft::Vector2& position);

	void SetTarget(const Craft::Vector2& target,
		const std::vector<std::vector<int>>& grid);
	bool HasPath() const;
	void SetGameLevel(GameLevel* level);
	void ClearPath();
	void SetTargetActor(const std::shared_ptr<Actor>& actor);
	const std::weak_ptr<Actor> GetTargetActor();
	bool HasTargetActor() const;
	Craft::Vector2 CalculateSurroundPosition();
	inline int GetSurroundSlot() const { return surroundSlot; }
	void ClearTargetActor();
	void SetSurroundSlot(int slot);
	const CrowdVelocity& GetVelocity() const;
private:
	// 이벤트 함수 오버라이드.
	virtual void Tick(float deltaTime) override;

	// 충돌 이벤트 함수 오버라이드.
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;
	CrowdVelocity CalculateSeparation();
	CrowdVelocity CalculateCohesion();
	CrowdVelocity CalculateAlignment();
	CrowdVelocity CalculateBoidsSteering();
	CrowdVelocity CalculatePathDirection();
	CrowdVelocity CalculateFinalDirection();
	void DebugDraw();
private:
	Craft::Astar astar;
	float moveSpeed = 2.0f;
	int currentPathIndex = 1;
	std::vector<Craft::Vector2> path;
	Craft::Vector2 target;
	GameLevel* gameLevel = nullptr;
	std::weak_ptr<Actor> targetActor;
	int surroundSlot = -1;
	float positionX = 0.0f;
	float positionY = 0.0f;

	CrowdVelocity velocity{ 0.0f, 0.0f };
	float maxSpeed = 1.0f;
	float separationWeight = 1.5f;
	float alignmentWeight = 1.0f;
	float cohesionWeight = 1.0f;

	float debugElapsedTime = 0.0f;
	bool showDebugPath = false;
};

