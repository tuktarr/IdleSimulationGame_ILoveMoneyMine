#include "EventManager.h"

EventManager::EventManager()
	: durationTimer(5.0f), nextEventTimer(30.0f)
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
	NotifySubscribers();
}
