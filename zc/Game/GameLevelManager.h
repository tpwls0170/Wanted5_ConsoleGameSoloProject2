#pragma once
#include <Engine/Engine.h>
#include <vector>

// 레벨 관리에 사용할 상태 열거형.
enum class State
{
	TitleMenu = 0,
	GamePlay = 1,
	GameOver = 2,
	Length
};

class GameLevelManager : public Craft::Engine
{
public:
	GameLevelManager();
	~GameLevelManager() = default;
	// 메뉴/게임 레벨을 전환하는 함수.
	void ToggleMenu(State currentState, State nextState);
	void ReSetGame();
private:
	// 메뉴 레벨과 게임 레벨을 관리할 배열.
	std::vector<std::shared_ptr<Craft::Level>> levelList;
	// 현재 활성화된 레벨의 상태를 나타내는 변수.
	State nextState = State::GamePlay;
	State currentState = State::GamePlay;
};

