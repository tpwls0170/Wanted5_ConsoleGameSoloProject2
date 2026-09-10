#include "GameOver.h"
#include <Game/GameLevelManager.h>
#include <Render/Renderer.h>
#include <Input/Input.h>

using namespace Craft;
GameOver::GameOver()
{
	title = {
		L"        █████ █   █ ████            ",
		L"        █     ██  █ █   █           ",
		L"████    ████  █ █ █ █   █    ████   ",
		L"        █     █  ██ █   █           ",
		L"        █████ █   █ ████            ",
	};
}

void GameOver::Tick(float deltaTime)
{
    DebugDraw();
}

void GameOver::DebugDraw()
{
    const int screenWidth =
        static_cast<int>(Engine::Get().GetWidth());

    const int screenHeight =
        static_cast<int>(Engine::Get().GetHeight());

    const int titleHeight =
        static_cast<int>(title.size());

    // 타이틀 전체의 세로 시작 위치
    const int startY =
        ((screenHeight - titleHeight) / 2) + 3;

    for (int y = 0; y < titleHeight; ++y)
    {
        const int titleWidth =
            static_cast<int>(title[y].size());

        // 각 줄을 가로 중앙 정렬
        const int startX =
            (screenWidth - titleWidth) / 2;

        Renderer::Get().Submit(
            title[y],
            Vector2(startX, startY + y),
            Craft::Color::Red
        );
    }
}
