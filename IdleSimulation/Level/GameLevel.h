#pragma once

#include "Level/Level.h"

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

	// TODO: 충돌 판정 처리함수
};