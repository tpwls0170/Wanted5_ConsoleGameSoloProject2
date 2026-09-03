#include <Engine/Engine.h>
#include <Windows.h>
#include <Level/GameLevel.h>
#include <fcntl.h>
#include <io.h>

using namespace Craft;
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 6;

wchar_t map[MAP_HEIGHT][MAP_WIDTH] =
{
    { L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#' },
    { L'#', L' ', L' ', L' ', L' ', L'P', L' ', L' ', L' ', L'#' },
    { L'#', L' ', L'#', L'#', L' ', L' ', L' ', L'#', L' ', L'#' },
    { L'#', L' ', L' ', L' ', L' ', L'Z', L' ', L' ', L' ', L'#' },
    { L'#', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L'#' },
    { L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#' }
};


void PrintEmoji(const wchar_t* text)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD written = 0;

    WriteConsoleW(
        console,
        text,
        lstrlenW(text),
        &written,
        nullptr
    );
}

void RenderMap()
{
    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            switch (map[y][x])
            {
            case L'#':
                PrintEmoji(L"🧱");
                break;

            case L'P':
                PrintEmoji(L"🧑");
                break;

            case L'Z':
                PrintEmoji(L"🧟");
                break;

            default:
                PrintEmoji(L"  ");
                break;
            }
        }

        PrintEmoji(L"\n");
    }
}

int main()
{
    //RenderMap();

	Engine engine;
    engine.AddNewLevel<GameLevel>();
	engine.Run();
}