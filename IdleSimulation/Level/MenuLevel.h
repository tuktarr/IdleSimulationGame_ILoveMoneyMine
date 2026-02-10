#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"
#include <string>

namespace Wanted
{
    class MenuLevel : public Level
    {
	    RTTI_DECLARATIONS(MenuLevel, Level)

    public:
        MenuLevel();
        virtual ~MenuLevel();

        virtual void Tick(float deltaTime) override;
        virtual void Draw() override;
    private:
        void HandleInput();

        // 버튼 영역 체크 함수 (점과 사각형의 충돌)
        bool IsInside(Vector2 mousePos, Vector2 btnPos, int width, int height);

    private:
        // 버튼 위치 및 크기 설정
        Vector2 newGamePos = { 45, 12 };
        Vector2 loadGamePos = { 45, 15 };
        int btnWidth = 22;
        int btnHeight = 2;

        // 세이브 파일 존재 여부
        bool bHasSaveFile = false;
    };
}

