#pragma once
#include "Actor/Actor.h"

using namespace Wanted;

class HelpManager : public Actor
{
	RTTI_DECLARATIONS(HelpManager, Actor)

public:
	HelpManager();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// 도움말 창 열기/닫기 토글
	void ToggleHelp() { isVisible = !isVisible; }
	bool IsOpen() const { return isVisible; }

	// 명시적 제어함수
	void Open() { isVisible = true; }
	void Close() { isVisible = false; }
private:
	bool isVisible = false;

	// UI 설정
	static const int SCREEN_WIDTH = 110;
	static const int SCREEN_HEIGHT = 28;
};

