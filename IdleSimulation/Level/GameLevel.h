#pragma once

#include "Level/Level.h"
#include "Util/Timer.h"
#include "Actor/AdManager.h"
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
	// 입력 로직
	void HandleInput();
	// 로그표시 함수
	void RenderUI();
	// 광산 그리드 배치 및 초기화 함수
	void InitializeMines();
	// 로그 설정 함수
	void SetLog(const std::string& message);

	// 파일 저장하기 / 불러오기
	void SaveGame();
	void LoadAndCalcOfflineReward();

	// 광고 보상 계산 함수
	long long CalculateAdReward();
private:
	// 플레이어 객체
	Player* player = nullptr;

	// GameLevel의 모든 로그 담을 객체
	std::string currentLog = "준비 완료...";
	Timer logTimer;

	// 게임 종료 조건 플래그
	bool isGameClear = false;
	
	// 광고 매니저 추가
	AdManager* adManager = nullptr;
};