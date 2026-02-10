#pragma once
#include "Actor/Actor.h"
#include "Util/Timer.h"
#include <functional>


using namespace Wanted;

class AdManager : public Actor
{
	RTTI_DECLARATIONS(AdManager, Actor);
	
public:
	enum class EAdState
	{
		HIDDEN, // 쿨타임 중 (화면에 안보임)
		READY, // 광고 볼 준비 완료 (버튼 보임)
		PLAYING, // 광고 재생 중 (입력 차단)
		FINISHED // 광고 끝남 (보상 수령 대기)
	};
	using OnAdFinishedFunc = std::function<void()>;

public:
	AdManager();
	~AdManager();
	// 초기화: 보상 지급 함수(콜백)를 등록
	void Init(OnAdFinishedFunc callback);

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
	
	// 광고창 클릭 처리 함수
	bool HandleClick(int x, int y);

	// 현재 광고 재생 중인지 확인 (입력 차단용)
	bool IsPlayingAd() const { return currentState == EAdState::PLAYING; }
	
	void SetDisabledByEvent(bool bDisabled) { bDisabledByEvent = bDisabled;}

private:
	// 광고버튼 이벤트로 인한 비활성화 상태 플래그
	bool bDisabledByEvent = false;

	static const int UI_X = 70;
	static const int UI_Y = 20;
	static const int BUTTON_WIDTH = 10; // 버튼 크기

	const float AD_DURATION = 5.0f; // 광고 지속 시간
	const float AD_COOLDOWN = 5.0f; // 광고 다시 뜨는 시간
	
	EAdState currentState; // 현재 상태
	Timer adStateTimer; // 상태 관리용 타이머
	OnAdFinishedFunc onAdFinishedCallback; // 보상 지급 델리게이트

private:
	// 상태 / 충돌 체크 함수
	bool IsMouseOver(int mouseX, int mouseY) const;
};

