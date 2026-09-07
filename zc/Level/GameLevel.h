#pragma once
#include <Level/Level.h>
#include <Math/Vector2.h>
#include <vector>
#include <Algorithm/QuadTree.h>
#include <Actor/Actor.h>

class Citizen;
class Zombie;
class PoliceActor;
class Gimmick_Exit;
class GameLevel : public Craft::Level
{
	struct GameSetting
	{
		int stageWidth = 0;
		int stageHight = 0;
		int zombieMaxCount = 0;
		int citizenMaxCount = 0;
		int policeMaxCount = 0;
		int soldierMaxCount = 0;
		int shelterMaxCount = 0;
		int wallMaxCount = 0;
	};

	enum BuildType
	{
		None,
		Wall,
		Police,
		Exit
	};

	struct ButtonUI
	{
		Craft::Vector2 position;
		int width;
		int height;
		BuildType type;
	};

public:
	bool IsCitizenAt(
		const Craft::Vector2& position,
		const Citizen* except);
	void CreateZombie(const Craft::Vector2 position, bool isInfection);
	void UpdateQuadTree(Craft::Actor* actor);
private:
	virtual void OnInitialized();
	virtual void Tick(float deltaTime) override;

	void initCreateMap();
	void LoadGameLevelSetting();
	void LoadGameLevelMap();
	void initCreateActor();
	void DrawUI();
	void DrawButtonUI(const ButtonUI& button);
	void SelectActor();
	void BuildActor();
	std::wstring GetBuildTypeName(BuildType type);
	void UpdateCitizenAI();
	void UpdateZombieAI();
	void UpdatePoliceAI();
	void RemoveDestroyedCitizens();
private:
	GameSetting gameSetting;
	int currentStage = 1;
	// 0 none 1 시민 2 탈출구 3 벽 4 경찰 5 좀비
	std::vector<std::vector<int>> actorPositionVec;
	std::vector<Craft::Vector2> exitPositions;
	std::vector<std::shared_ptr<Citizen>> citizens;
	std::vector<std::shared_ptr<Zombie>> zombies;
	std::vector<std::shared_ptr<PoliceActor>> polices;
	std::vector<std::shared_ptr<Gimmick_Exit>> gimmickExits;
	Craft::Vector2 mousePosition = Craft::Vector2::Zero;
	BuildType selectBuildType = BuildType::None;
	ButtonUI startButton;
	ButtonUI exitButton;
	ButtonUI wallButton;
	ButtonUI policeButton;

	std::unique_ptr<Craft::QuadTree> quadTree;
	bool gameStart = false;
	int zombieCount = 0;
	int citizenCount = 0;
	int policeCount = 0;
	int soldierCount = 0;
	int shelterCount = 0;
	int wallCount = 0;
};

