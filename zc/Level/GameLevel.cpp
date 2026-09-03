#include "GameLevel.h"
#include <Actor/Character/Citizen.h>
#include <Actor/Character/PoliceActor.h>
#include <Actor/Enemy/Zombie.h>
#include <Actor/Gimmick/Gimmick_Exit.h>
#include <Actor/Gimmick/Gimmick_Wall.h>
#include <cassert>
#include <Input/Input.h>
#include <random>
#include <Render/Renderer.h>

#include <iostream>
using namespace Craft;
void GameLevel::OnInitialized()
{
	Level::OnInitialized();
	
	LoadGameLevelSetting();
	actorPositionVec.resize(gameSetting.stageHight);

	for (auto& row : actorPositionVec)
	{
		row.resize(gameSetting.stageWidth, 0);
	}
	initCreateMap();
}

void GameLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
	DrawUI();

	if(Input::Get().GetKeyDown(VK_LBUTTON))
	{
		mousePosition = Input::Get().GetMousePosition();
		if (mousePosition.y > gameSetting.stageHight)
		{
			SelectActor();
		}
		else
		{
			BuildActor();
		}
	}
}

void GameLevel::initCreateMap()
{
	LoadGameLevelMap();
	initCreateActor();
}

void GameLevel::LoadGameLevelSetting()
{
	// 최종 경로 조립.
	std::string path = std::string("../Resources/Assets/") + "Stage" + std::to_string(currentStage) + "_Setting.txt";
	// 파일 열기 (C-Style).
	FILE* file = nullptr;
	fopen_s(&file, path.c_str(), "rt");
	if (!file)
	{
		assert(false && "failed to open a sokoban stage file.");

		// 디버그 모드에서 강제 중단 시키는 기능.
		__debugbreak();
		return;
	}

	// 데이터 읽어오기.
	const int bufferSize = 2048;
	char buffer[bufferSize] = {};

	size_t readSize
		= fread(buffer, sizeof(char), bufferSize, file);

	// 값 저장을 위해 서식 해석 (파싱-Parsing).
	// 문자열 자르기(Split).
	char* context = nullptr;
	char* token = nullptr;
	// 파일에서 읽은 전체 문자열을 개행(\n)문자 기준으로 처음 자르기.
	token = strtok_s(buffer, "\n", &context);

	// 반복해서 자르기
	while (token)
	{
		// 공백 전까지 읽은 문자열을 저장할 변수.
		char key[20] = {};

		// 포맷을 지정한 문자열 읽기.
		// 공백 문자를 만나면 그 전까지 읽어서 저장.
		sscanf_s(token, "%s", key, 20);

		// 키 값을 비교해서 값 설정.
		if (strcmp(key, "zombieMaxCount") == 0)
		{
			sscanf_s(token, "zombieMaxCount = %d", &gameSetting.zombieMaxCount);
		}
		else if (strcmp(key, "citizenMaxCount") == 0)
		{
			sscanf_s(token, "citizenMaxCount = %d", &gameSetting.citizenMaxCount);
		}
		else if (strcmp(key, "policeMaxCount") == 0)
		{
			sscanf_s(token, "policeMaxCount = %d", &gameSetting.policeMaxCount);
		}
		else if (strcmp(key, "soldierMaxCount") == 0)
		{
			sscanf_s(token, "soldierMaxCount = %d", &gameSetting.soldierMaxCount);
		}
		else if (strcmp(key, "shelterMaxCount") == 0)
		{
			sscanf_s(token, "shelterMaxCount = %d", &gameSetting.shelterMaxCount);
		}
		else if (strcmp(key, "wallMaxCount") == 0)
		{
			sscanf_s(token, "wallMaxCount = %d", &gameSetting.wallMaxCount);
		}
		else if (strcmp(key, "stageWidth") == 0)
		{
			sscanf_s(token, "stageWidth = %d", &gameSetting.stageWidth);
		}
		else if (strcmp(key, "stageHight") == 0)
		{
			sscanf_s(token, "stageHight = %d", &gameSetting.stageHight);
		}
		// 나머지 문자열 자르기(개행 문자 기준으로).
		token = strtok_s(nullptr, "\n", &context);
	}

	// 파일 닫기.
	fclose(file);
	file = nullptr;
}

void GameLevel::LoadGameLevelMap()
{
	//// 최종 경로 조립.
	//std::string path = std::string("../Assets/") + "Stage" + std::to_string(currentStage) + "_Map";
	//// 파일 열기 (C-Style).
	//FILE* file = nullptr;
	//fopen_s(&file, path.c_str(), "rt");
	//if (!file)
	//{
	//	assert(false && "failed to open a sokoban stage file.");
	//	return;
	//}
}

void GameLevel::initCreateActor()
{
	int num = 0;
	int x = 0;
	int y = 0;


	for (int i = 0; i < gameSetting.citizenMaxCount; ++i)
	{
		do
		{
			num = rand();
			x = (int)num % gameSetting.stageWidth;
			y = (int)num % gameSetting.stageHight;
		} while (actorPositionVec[y][x] != 0);

		actorPositionVec[y][x] = 1;
		Vector2 citizenPosition = { x,y };
		auto citizen = SpawnActor<Citizen>(citizenPosition);
		citizens.emplace_back(citizen);
	}

	for (int i = 0; i < gameSetting.zombieMaxCount; ++i)
	{
		do
		{
			num = rand();
			x = (int)num % gameSetting.stageWidth;
			y = (int)num % gameSetting.stageHight;
		} while (actorPositionVec[y][x] != 0);

		actorPositionVec[y][x] = 5;
		Vector2 zombiePosition = { x,y };
		auto zombie = SpawnActor<Zombie>(zombiePosition);
	}
}

void GameLevel::DrawUI()
{
	const int uiX = 50;
	const int uiY = 35;
	const int buttonWidth = 11;
	const int buttonHight = 3;

	Craft::Renderer::Get().Submit(
		L"===================================================================================",
		Craft::Vector2(0, uiY),
		Craft::Color::White
	);

	exitButton = { Craft::Vector2(2, uiY + 1) , buttonWidth, buttonHight, BuildType::Exit };
	Craft::Renderer::Get().Submit(
		L"🚪 EXIT",
		Craft::Vector2(exitButton.position.x + 1, exitButton.position.y + 1),
		Craft::Color::White, 2
	);
	DrawButtonUI(exitButton);

	wallButton = { Craft::Vector2(18, uiY + 1) , buttonWidth, buttonHight, BuildType::Wall };
	Craft::Renderer::Get().Submit(
		L"🚪 WALL",
		Craft::Vector2(wallButton.position.x + 1, wallButton.position.y + 1),
		Craft::Color::White, 2
	);
	DrawButtonUI(wallButton);

	policeButton = { Craft::Vector2(34, uiY + 1) , buttonWidth, buttonHight, BuildType::Police };
	Craft::Renderer::Get().Submit(
		L"👮 POLICE",
		Craft::Vector2(policeButton.position.x + 1, policeButton.position.y + 1),
		Craft::Color::White, 2
	);
	DrawButtonUI(policeButton);

	Craft::Renderer::Get().Submit(
		L"SelectActor : " + GetBuildTypeName(selectBuildType),
		Craft::Vector2(uiX, uiY + 2),
		Craft::Color::White, 2
	);
	Craft::Renderer::Get().Submit(
		L"===================================================================================",
		Craft::Vector2(0, uiY + 4),
		Craft::Color::White
	);
}

void GameLevel::DrawButtonUI(const ButtonUI& button)
{
	const int x = button.position.x;
	const int y = button.position.y;

	const int right = x + button.width - 1;
	const int bottom = y + button.height - 1;

	// 위쪽
	for (int i = x; i <= right; ++i)
	{
		Renderer::Get().Submit(
			L"-",
			Vector2(i, y),
			Color::White
		);
	}

	// 아래쪽
	for (int i = x; i <= right; ++i)
	{
		Renderer::Get().Submit(
			L"-",
			Vector2(i, bottom),
			Color::White
		);
	}

	// 왼쪽
	for (int i = y + 1; i < bottom; ++i)
	{
		Renderer::Get().Submit(
			L"|",
			Vector2(x, i),
			Color::White
		);
	}

	// 오른쪽
	for (int i = y + 1; i < bottom; ++i)
	{
		Renderer::Get().Submit(
			L"|",
			Vector2(right, i),
			Color::White
		);
	}
	
	// 모서리
	Renderer::Get().Submit(L"+", Vector2(x, y), Color::White);
	Renderer::Get().Submit(L"+", Vector2(right, y), Color::White);
	Renderer::Get().Submit(L"+", Vector2(x, bottom), Color::White);
	Renderer::Get().Submit(L"+", Vector2(right, bottom), Color::White);
}

void GameLevel::SelectActor()
{
	if (mousePosition.x >= exitButton.position.x &&
		mousePosition.x < exitButton.position.x + exitButton.width &&
		mousePosition.y >= exitButton.position.y &&
		mousePosition.y < exitButton.position.y + exitButton.height)
	{
		selectBuildType = BuildType::Exit;
	}
	else if (mousePosition.x >= wallButton.position.x &&
		mousePosition.x < wallButton.position.x + wallButton.width &&
		mousePosition.y >= wallButton.position.y &&
		mousePosition.y < wallButton.position.y + wallButton.height)
	{
		selectBuildType = BuildType::Wall;
	}
	else if (mousePosition.x >= policeButton.position.x &&
		mousePosition.x < policeButton.position.x + policeButton.width &&
		mousePosition.y >= policeButton.position.y &&
		mousePosition.y < policeButton.position.y + policeButton.height)
	{
		selectBuildType = BuildType::Police;
	}
}

void GameLevel::BuildActor()
{
	if (actorPositionVec[mousePosition.y][mousePosition.x] == 0)
	{
		switch (selectBuildType)
		{
		case GameLevel::None:
			break;
		case GameLevel::Wall:
		{
			if (gameSetting.wallMaxCount > wallCount)
			{
				actorPositionVec[mousePosition.y][mousePosition.x] = 3;
				Vector2 gimmick_wallPosition = { mousePosition.x,mousePosition.y };
				SpawnActor<Gimmick_Wall>(gimmick_wallPosition);
				wallCount++;
			}
		}
			break;
		case GameLevel::Police:
		{
			if (gameSetting.policeMaxCount > policeCount)
			{
				actorPositionVec[mousePosition.y][mousePosition.x] = 4;
				Vector2 policePosition = { mousePosition.x,mousePosition.y };
				SpawnActor<PoliceActor>(policePosition);
				policeCount++;
			}
		}
			break;
		case GameLevel::Exit:
		{
			if (gameSetting.shelterMaxCount > shelterCount)
			{
				actorPositionVec[mousePosition.y][mousePosition.x] = 2;
				Vector2 gimmick_exitPosition = { mousePosition.x,mousePosition.y };
				exitPositions.push_back(gimmick_exitPosition);
				SpawnActor<Gimmick_Exit>(gimmick_exitPosition);
				shelterCount++;

				for (auto citizen : citizens)
				{
					citizen->SetTarget(exitPositions[0], actorPositionVec);
				}
			}
		}
			break;
		default:
			break;
		}
	}
	
}

std::wstring GameLevel::GetBuildTypeName(BuildType type)
{
	switch (type)
	{
	case BuildType::Exit:
		return L"Exit";

	case BuildType::Wall:
		return L"Wall";

	case BuildType::Police:
		return L"Police";

	case BuildType::None:
		return L"None";
	}

	return L"None";
}
