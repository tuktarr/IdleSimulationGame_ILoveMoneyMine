#pragma once

#include "Level/Level.h"
#include "Util/Timer.h"
#include <string>

#include <stdio.h>
#include <time.h>

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
	void InitializeMines(); // 광산 그리드 배치 및 초기화 함수
	void SetLog(const std::string& message); // 로그 설정 함수
	void SaveGame();
	void LoadAndCalcOfflineReward();

private:
	// 플레이어 객체
	Player* player = nullptr;

	// GameLevel의 모든 로그 담을 객체
	std::string currentLog = "준비 완료...";
	Timer logTimer;

	// 게임 종료 조건 플래그
	bool isGameClear = false;
};