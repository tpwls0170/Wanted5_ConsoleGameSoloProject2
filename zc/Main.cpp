//#include <Engine/Engine.h>
#include <Game/GameLevelManager.h>
#include <Level/GameLevel.h>
#include <cwchar>

using namespace Craft;

int main()
{
	//Engine engine;
    //engine.AddNewLevel<GameLevel>();
	//engine.Run();

	SetConsoleTitleA("ZombieCity");

	GameLevelManager game;
	game.Run();
}