#include <Engine/Engine.h>
#include <Windows.h>
#include <Level/GameLevel.h>

using namespace Craft;

int main()
{
	Engine engine;
    engine.AddNewLevel<GameLevel>();
	engine.Run();
}