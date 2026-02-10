#pragma once

#include "Actor/Actor.h"
#include <functional>
#include <vector>
#include "Util/Timer.h"

using namespace Wanted;
class EventManager : public Actor
{
	RTTI_DECLARATIONS(EventManager,Actor)
	
public:
	enum class EEventType
	{
		NONE,
		EARTHQUAKE,    // 지진: 일시 정지
		GOLD_RUSH,     // 피버타임: 속도 증가
		MAX_COUNT
	};

	EventManager();

	// 이벤트를 수신할 델리게이트 타입 정의
	using OnGameEventDelegate = std::function<void(EEventType)>;

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// 구독 등록 함수
	void Subscribe(OnGameEventDelegate callback) { subscribers.push_back(callback); }

	// 현재 이벤트를 가져오는 함수
	EEventType GetCurrentEvent() const { return currentEvent; }

private:
	void TriggerRandomEvent();
	void NotifySubscribers();
	void EndEvent();

private:
	EEventType currentEvent = EEventType::NONE;
	std::vector<OnGameEventDelegate> subscribers;

	Timer durationTimer;
	Timer nextEventTimer;
};

