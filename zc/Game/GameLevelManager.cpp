#include "GameLevelManager.h"
#include <Level/TitleLevel.h>
#include <Level/GameLevel.h>
#include <Level/GameOver.h>

GameLevelManager::GameLevelManager()
{
	levelList.emplace_back(std::make_shared<TitleLevel>());
	levelList.emplace_back(std::make_shared<GameLevel>());
	levelList.emplace_back(std::make_shared<GameOver>());

	// 시작 상태 설정.
	nextState = State::TitleMenu;

	// 게임 시작시 활성화할 레벨 설정.
	mainLevel = levelList[(int)nextState];
}

void GameLevelManager::ToggleMenu(State currentState, State nextState)
{
	int stateIndex = static_cast<int>(nextState);
	// 레벨 설정 및 상태 값 업데이트.
	mainLevel = levelList[stateIndex];
	this->nextState = static_cast<State>(stateIndex);
}

void GameLevelManager::ReSetGame()
{}
