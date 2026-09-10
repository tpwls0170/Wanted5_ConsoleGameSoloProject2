#pragma once
#include <Level/Level.h>

class GameOver : public Craft::Level
{
public:
	GameOver();
private:
	virtual void Tick(float deltaTime) override;
	void DebugDraw();

private:
	std::vector<std::wstring> title;
};

