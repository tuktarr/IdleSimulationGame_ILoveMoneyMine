#include "AdManager.h"
#include "Render/Renderer.h"
#include "Core/Input.h"

AdManager::AdManager()
	: currentState(EAdState::HIDDEN), adStateTimer(AD_COOLDOWN)
{
}

AdManager::~AdManager()
{
}

void AdManager::Init(OnAdFinishedFunc callback)
{
	onAdFinishedCallback = callback;

	currentState = EAdState::HIDDEN;
	adStateTimer.SetTargetTime(AD_COOLDOWN);
	adStateTimer.Reset();
}

void AdManager::Tick(float deltaTime)
{
	adStateTimer.Tick(deltaTime);

	// Timer의 시간 변화에 따른 상태변환
	switch (currentState)
	{
	case EAdState::HIDDEN:
		// 쿨타임이 다 찼으면 -> READY 상태로 전환
		if (adStateTimer.IsTimeOut())
		{
			currentState = EAdState::READY;
		}
		break;

	case EAdState::PLAYING:
		// 광고 시간이 다 됐으면 -> FINISHED 상태로 전환
		if (adStateTimer.IsTimeOut())
		{
			currentState = EAdState::FINISHED;
		}
		break;

	case EAdState::READY:
	case EAdState::FINISHED:
		break;
	}
}

void AdManager::Draw()
{
	const int UI_SORT_ORDER = 100;
	Vector2 mousePos = Input::Get().GetMousePosition();
	bool isHovered = IsMouseOver((int)mousePos.x, (int)mousePos.y);

	switch (currentState)
	{
	case EAdState::HIDDEN:
		// (필요하다면 남은 시간을 표시할 수도 있음)
		break;

	case EAdState::READY:
	{
		// [광고 보기] 버튼
		Color ReadybtnColor = isHovered ? Color::White : Color::Yellow;
		Renderer::Get().Submit("[  AD  ]", Vector2(UI_X, UI_Y), ReadybtnColor, UI_SORT_ORDER);
		break;
	}

	case EAdState::PLAYING:
	{
		// 진행률 게이지바 문자열 생성
		// "[#####     ]"
		float progress = adStateTimer.GetProgress(); // 0.0 ~ 1.0
		int barLength = 10;
		int currentFill = (int)(progress * barLength);

		std::string progressBar = "[";
		for (int i = 0; i < barLength; ++i)
		{
			if (i < currentFill)
				progressBar += "#";
			else
				progressBar += " ";
		}
		progressBar += "]";

		// 게이지바 그리기 (흰색)
		Renderer::Get().Submit(progressBar, Vector2(UI_X, UI_Y), Color::White, UI_SORT_ORDER);
		break;
	}

	case EAdState::FINISHED:
	{
		// [보상 받기] 버튼
		Color FinishedbtnColor = isHovered ? Color::White : Color::Green;
		Renderer::Get().Submit("[ GET $ ]", Vector2(UI_X, UI_Y), FinishedbtnColor, UI_SORT_ORDER);
		break;

	}
	}
}

bool AdManager::HandleClick(int x, int y)
{
	if (currentState == EAdState::PLAYING)
	{
		return true;
	}
	if (currentState == EAdState::HIDDEN)
	{
		return false;
	}

	if (IsMouseOver(x, y))
	{
		if (currentState == EAdState::READY)
		{
			currentState = EAdState::PLAYING;
			adStateTimer.SetTargetTime(AD_DURATION);
			adStateTimer.Reset();
			return true;
		}
		else if (currentState == EAdState::FINISHED)
		{
			if (onAdFinishedCallback)
			{
				onAdFinishedCallback();
			}

			currentState = EAdState::HIDDEN;
			adStateTimer.SetTargetTime(AD_COOLDOWN);
			adStateTimer.Reset();
			return true;
		}
	}
	return false;
}

bool AdManager::IsMouseOver(int mouseX, int mouseY) const
{
	// 상태 체크
	if (currentState != EAdState::READY && currentState != EAdState::FINISHED)
	{
		return false;
	}
	// 범위 체크
	return (mouseX >= UI_X && mouseX <= UI_X + BUTTON_WIDTH && mouseY == UI_Y);
}
