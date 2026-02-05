#include "GameLevel.h"
#include "Actor/Mine.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include <iostream>
#include <windows.h>

void LogCursor(int x, int y)
{
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

GameLevel::GameLevel()
{
    // 일단 힙에 생성
    Mine* testMine = new Mine(Mine::EMineType::Copper, Vector2(18, 9));
    AddNewActor(testMine);
}
GameLevel::~GameLevel()
{
}

void GameLevel::Tick(float deltaTime)
{
    Level::Tick(deltaTime);

    if (Input::Get().GetButtonDown(VK_ESCAPE))
    {
        Engine::Get().QuitEngine();
        return;
    }

    // 마우스 왼쪽 클릭 감지
    if (Input::Get().GetButtonDown(VK_LBUTTON))
    {
        // 마우스 좌표 가져오기
        Vector2 mousePos = Input::Get().GetMousePosition();

        // 로그 출력 (화면 하단 20번째 줄에 출력)
        LogCursor(0, 20);
        std::cout << "                                            "; // 줄 지우기
        LogCursor(0, 20);
        std::cout << "[Click] X: " << mousePos.x << ", Y: " << mousePos.y;

        // 충돌 판정 (광산을 클릭했는가?)

        for (Actor* actor : actors)
        {
            Mine* mine = actor->As<Mine>();

            if (mine != nullptr)
            {
                Vector2 pos = mine->GetPosition();

                // 간단한 좌표 일치 충돌 체크 (광산 크기가 1x1이라 가정)
                // int 형변환을 통해 정확한 정수 좌표 비교
                if (mine && mine->IsSelected(mousePos))
                {
                    const Mine::MineData& data = mine->GetData();

                    std::cout << " -> [HIT!] " << data.name << " 선택됨! ("
                        << data.basicIncome << "원 생산)";
                    // TODO : 메뉴 오픈 로직 등이 여기에 들어감
                }
            }
        }
    }
}
