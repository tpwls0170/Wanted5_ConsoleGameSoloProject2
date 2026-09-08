#pragma once
#include <Level/Level.h>

class TitleLevel : public Craft::Level
{
public:
	TitleLevel();
private:
	virtual void Tick(float deltaTime) override;
	void DebugDraw();

private:
	std::vector<std::wstring> title;
};

