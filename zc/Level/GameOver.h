#pragma once
#include <Level/Level.h>

class GameOver : public Craft::Level
{
public:
	GameOver();
private:
	void Draw();

private:
	std::vector<std::wstring> title;
};

