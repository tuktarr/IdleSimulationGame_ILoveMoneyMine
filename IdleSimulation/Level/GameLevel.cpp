#include "GameLevel.h"
#include "Actor/Mine.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Actor/Player.h"
#include <iostream>
#include <windows.h>
#include "Render/Renderer.h"

GameLevel::GameLevel()
{
    // 어처피 위치정보를 가질 필요가 없으므로 그냥 생성자로 생성
    player = new Player();
    AddNewActor(player);

    Mine* copperMine = new Mine(Mine::EMineType::Copper, Vector2(18, 9));
    copperMine->Purchase(); // 구리는 처음에 하나 주고 시작
    // 콜백 연결 : 광산이 한 바퀴 돌면 플레이어의 자산을 추가함
    copperMine->SetOnCycleComplete([this](long long income) 
        {
            if (player)
            {
                player->AddGold(static_cast<int>(income));
            }
        });
    
    
    player->AddMineList(copperMine); // 플레이어 소유 목록에 추가
    AddNewActor(copperMine);

    // 초기 자금
    player->AddGold(0);
}
GameLevel::~GameLevel()
{
}

void GameLevel::Tick(float deltaTime)
{
    Level::Tick(deltaTime);

    // 입력 후, 데이터 계속 업데이트
    HandleInput();

    // 로그 정보
    RenderUI();
    
}

void GameLevel::HandleInput()
{
    if (Input::Get().GetButtonDown(VK_ESCAPE))
    {
        Engine::Get().QuitEngine();
        return;
    }

    if (Input::Get().GetButtonDown(VK_LBUTTON))
    {
        // 마우스 좌표 가져오기
        Vector2 mousePos = Input::Get().GetMousePosition();

        // 충돌 판정 (광산을 클릭했는가?)
        for (Actor* actor : actors)
        {
            Mine* mine = actor->As<Mine>();
            if (mine != nullptr && mine->IsSelected(mousePos))
            {
                // 선택된 Mine의 데이터를 저장해놓음
                const Mine::MineData& data = mine->GetData();

                if (!mine->IsPurchased())
                {
                    if (player->TryPurchase(data.purchasePrice))
                    {
                        // 광산 구매된 상태로 바꿈
                        mine->Purchase();
                        player->AddMineList(mine);

                        // 새로운 광산들도 Income이 들어오도록 연결
                        mine->SetOnCycleComplete([this](long long income)
                            {
                                player->AddGold(income);
                            });
                        // 로그
                        currentLog = data.name + "구입 성공!";
                    }
                    else
                    {
                        // 로그
                        long long lacking = data.purchasePrice - player->GetGold();
                        currentLog = "자산 부족! " + std::to_string(lacking) + "G가 더 필요합니다.";
                    }
                }
                else
                {
                    // 업그레이드 비용
                    long long currentCost = mine->GetUpgradePrice();

                    if (player->TryPurchase(currentCost))
                    {
                        mine->Upgrade();
                        currentLog = data.name + " Level Up! (LV." + std::to_string(mine->GetLevel()) +
                            ") 수입: " + std::to_string(mine->GetIncome()) + "G";
                    }
                    else
                    {
                        // 업그레이드 실패 로그
                        long long lacking = currentCost - player->GetGold();
                        currentLog = "골드 부족! 업그레이드에 " + std::to_string(lacking) + "G가 부족합니다.";
                    }
                }
            }
        }
    }
}

void GameLevel::RenderUI()
{
    // UI 출력 (매 프레임 자산 표시)
    // 자산 정보를 Submit으로 제출 (직접 cout 하지 않음)
    std::string goldStr = "현재 자산: " + std::to_string(player->GetGold()) + " G";
    Renderer::Get().Submit(goldStr, Vector2(0, 1), Color::Yellow, 10);

    // 로그 범위를 width에 초과해서 적용하면 랜더링도 깨짐
    // 시스템 로그
    std::string logStr = "LOG: " + currentLog;
    Renderer::Get().Submit(logStr, Vector2(0, 2), Color::White, 10);

    // 보유 광산 목록 및 레벨 표시

    int mineInfoNum = 35;
    Renderer::Get().Submit("-- 보유 광산 목록 --", Vector2(mineInfoNum, 1), Color::Cyan, 10);

    const std::vector<Mine*>& ownedMines = player->GetOwnedMines(); // Player 클래스에 getter가 있다고 가정

    if (ownedMines.empty())
    {
        Renderer::Get().Submit("보유한 광산이 없습니다.", Vector2(mineInfoNum, 2), Color::Gray, 10);
    }
    else
    {
        for (int i = 0; i < ownedMines.size(); ++i)
        {
            Mine* mine = ownedMines[i];
            const Mine::MineData& data = mine->GetData();

            // "광산이름 (Lv.X)" 형식으로 문자열 생성
            std::string mineInfo = "- " + data.name + " (Lv." + std::to_string(mine->GetLevel()) + ")";

            // i값을 이용해 세로로 나열 (X좌표 45, Y좌표 2부터 시작)
            Renderer::Get().Submit(mineInfo, Vector2(mineInfoNum, 2 + i), Color::Green, 10);
        }
    }
}

//TODO : 그리드 방식으로 맵에 비활성화 광산들 배치하기