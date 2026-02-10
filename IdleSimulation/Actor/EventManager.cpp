#include "EventManager.h"
#include "Render/Renderer.h"

EventManager::EventManager()
	: durationTimer(2.0f), nextEventTimer(10.0f)
{

}

void EventManager::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (currentEvent == EventManager::EEventType::NONE)
	{
		nextEventTimer.Tick(deltaTime);
		if (nextEventTimer.IsTimeOut())
		{
			TriggerRandomEvent();
		}
	}
	else
	{
		durationTimer.Tick(deltaTime);

		if (currentEvent == EEventType::EARTHQUAKE)
		{
			float shakeX = static_cast<float>(rand() % 3 - 1);
			float shakeY = static_cast<float>(rand() % 3 - 1);
			Renderer::Get().SetShakeOffset(Vector2(shakeX, shakeY));
		}

		if (durationTimer.IsTimeOut())
		{
			EndEvent();
		}
	}
}

void EventManager::Draw()
{
	super::Draw();

	if (currentEvent == EEventType::NONE)
	{
		return;
	}
}

void EventManager::TriggerRandomEvent()
{
	int random = rand() % 2 + 1;

	currentEvent = static_cast<EEventType>(random);

	if (currentEvent == EEventType::EARTHQUAKE)
	{
		// 지진은 짧게 (어지러움 방지 및 긴장감 극대화)
		durationTimer.SetTargetTime(2.0f);
	}
	else if (currentEvent == EEventType::GOLD_RUSH)
	{
		// 골드러쉬는 길게 (수익 극대화의 재미)
		durationTimer.SetTargetTime(10.0f);
	}

	durationTimer.Reset();
	NotifySubscribers();
}

void EventManager::NotifySubscribers()
{
	for (auto& callback : subscribers)
	{
		callback(currentEvent);
	}
}

void EventManager::EndEvent()
{
	currentEvent = EEventType::NONE;
	nextEventTimer.Reset();

	// 이벤트 종료 시 흔들림 초기화
	Renderer::Get().SetShakeOffset(Vector2::Zero);
	NotifySubscribers();
}
