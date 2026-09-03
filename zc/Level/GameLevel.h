#pragma once
#include <Level/Level.h>
#include <Math/Vector2.h>
#include <vector>

class GameLevel : public Craft::Level
{
	struct GameSetting
	{
		int stageWidth = 0;
		int stageHight = 0;
		int zombieCount = 0;
		int citizenCount = 0;
		int policeCount = 0;
		int soldierCount = 0;
		int shelterCount = 0;
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
private:
	GameSetting gameSetting;
	int currentStage = 1;
	std::vector<std::vector<int>> actorPositionVec;
	std::vector<Craft::Vector2> exitPositions;
	Craft::Vector2 mousePosition = Craft::Vector2::Zero;
	BuildType selectBuildType = BuildType::None;
	ButtonUI exitButton;
	ButtonUI wallButton;
	ButtonUI policeButton;
};

