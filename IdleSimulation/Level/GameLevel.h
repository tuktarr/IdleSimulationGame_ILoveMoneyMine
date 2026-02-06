#pragma once

#include "Level/Level.h"
#include <string>

class Player;
using namespace Wanted;

// 마우스 클릭과 광산,상점의 충돌 처리 해주는 클래스
class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	~GameLevel();

private:
	virtual void Tick(float deltaTime) override;
	void HandleInput(); // 입력 로직
	void RenderUI(); // 로그표시 함수

private:
	Player* player; // 플레이어 객체
	std::string currentLog = "준비 완료...";
};