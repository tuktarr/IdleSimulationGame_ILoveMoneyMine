#include "GameLevel.h"
#include "Actor/Mine.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Actor/Player.h"
#include <windows.h>
#include "Render/Renderer.h"
#include "Util/Util.h"

GameLevel::GameLevel()
{
    // 어처피 위치정보를 가질 필요가 없으므로 그냥 생성자로 생성
    player = new Player();
    AddNewActor(player);

    // 그리드로 맵에 광산 배치
    InitializeMines();

    // 게임 시작 시 데이터 로드 및 오프라인 수익 계산
    LoadAndCalcOfflineReward();

    // 로그 타이머 설정
    logTimer.SetTargetTime(3.0f);
}
GameLevel::~GameLevel()
{
    SaveGame();
}

void GameLevel::Tick(float deltaTime)
{
    Level::Tick(deltaTime);

    // 입력 후, 데이터 계속 업데이트
    HandleInput();

    // 로그 정보
    RenderUI();
    
    if (!currentLog.empty())
    {
        logTimer.Tick(deltaTime);

        // 시간 되면 로그 삭제
        if (logTimer.IsTimeOut())
        {
            currentLog = ""; // 문자열 비우기
            logTimer.Reset();
        }
    }
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
                if (mine->GetMinetype() == Mine::EMineType::None || mine->GetMinetype() == Mine::EMineType::MaxCount)
                {
                    continue;
                }
                // 선택된 Mine의 데이터를 저장해놓음
                const Mine::MineData& data = mine->GetData();

                if (!mine->IsPurchased())
                {
                    if (player->TryPurchase(data.purchasePrice))
                    {
                        // 광산 구매된 상태로 바꿈
                        mine->Purchase();
                        player->AddMineList(mine);
                        // 로그
                        SetLog(data.name + "구입 성공!");
                    }
                    else
                    {
                        // 로그
                        long long lacking = data.purchasePrice - player->GetGold();
                        SetLog("자산 부족! " + std::to_string(lacking) + "G가 더 필요합니다.");
                    }
                }
                else
                {
                    // 업그레이드 비용
                    long long currentCost = mine->GetUpgradePrice();

                    if (player->TryPurchase(currentCost))
                    {
                        mine->Upgrade();
                        SetLog(data.name + " Level Up! (LV." + std::to_string(mine->GetLevel()) +
                            ") 수입: " + std::to_string(mine->GetIncome()) + "G");
                    }
                    else
                    {
                        // 업그레이드 실패 로그
                        long long lacking = currentCost - player->GetGold();
                        SetLog("골드 부족! 업그레이드에 " + std::to_string(lacking) + "G가 부족합니다.");
                    }
                }
                break;
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

    int mineInfoNum = 75;
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

            std::string speed = std::to_string(mine->GetTimer().GetTargetTime());
            speed = speed.substr(0, speed.find('.') + 3);
            std::string mineInfo = "- " + data.name + " (Lv." + std::to_string(mine->GetLevel()) + ")" + " [" + speed + "s]";
            
            // i값을 이용해 세로로 나열
            Renderer::Get().Submit(mineInfo, Vector2(mineInfoNum, 2 + i), Color::Green, 10);
        }
    }
}

void GameLevel::InitializeMines()
{
    // -- 3x3 그리드 배치 --
    const int START_X = 10;  // 그리드 시작 X
    const int START_Y = 6;   // 그리드 시작 Y
    const int GAP_X = 16;    // 가로 간격 (상자너비 12 + 여백 4)
    const int GAP_Y = 8;     // 세로 간격 (상자높이 5 + 여백 3)
    for (int i = 0; i < 9; i++)
    {
        int row = i / 3;
        int col = i % 3;

        Vector2 pos(static_cast<float>(START_X + col * GAP_X), static_cast<float>(START_Y + row * GAP_Y));
        Mine::EMineType potentialType = (i < (int)Mine::EMineType::MaxCount) ? static_cast<Mine::EMineType>(i + 1) : Mine::EMineType::None;

        Mine* slot = new Mine(potentialType, pos);

        //// 구리(첫번째)만 구매된 상태로 시작
        //if (potentialType == Mine::EMineType::Copper)
        //{
        //    slot->Purchase();
        //    player->AddMineList(slot);
        //}

        // 콜백 연결
        slot->SetOnCycleComplete([this](long long income) {
            if (player)
            {
                player->AddGold(income);
            }
            });

        AddNewActor(slot);
    }
}

void GameLevel::SetLog(const std::string& message)
{
    currentLog = message;
    logTimer.Reset();
}

void GameLevel::SaveGame()
{
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, "../Config/SaveData.txt", "wt");

    // 예외처리
    if (!file || err != 0)
    {
        SetLog("Failed to open SaveData");
        return;
    }

    // 플레이어 골드 저장
    long long currentGold = player->GetGold();
    fprintf(file, "%lld\n", currentGold);

    // 현재 시간 저장
    time_t now = time(nullptr);
    fprintf(file, "%lld\n", (long long)now);

    // 광산 정보 저장
    for (Actor* actor : actors)
    {
        Mine* mine = actor->As<Mine>();

        // 유효한 광산인 경우에만 저장
        if (mine != nullptr)
        {
            // 광산의 타입 순서대로 저장
            Mine::EMineType type = mine->GetMinetype();
            if (type == Mine::EMineType::None || type == Mine::EMineType::MaxCount)
            {
                continue;
            }

            int isPurchased = mine->IsPurchased() ? 1 : 0;
            int level = mine->GetLevel();

            fprintf(file, "% d % d\n", isPurchased, level);
        }
    }
    fclose(file);
}

// TODO 작업 : 메모장에 광산의 레벨과 생존을 숫자로 표기했는데 실행하면 다시 초기화가 되고 있음!
void GameLevel::LoadAndCalcOfflineReward()
{
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, "../Config/SaveData.txt", "rt");

    // 저장된 파일이 없으면 그냥 리턴
    if (err != 0 || file == nullptr)
    {
        SetLog("새로운 게임을 시작합니다.");

        player->AddGold(0);

        for (Actor* actor : actors)
        {
            Mine* mine = actor->As<Mine>();
            if (mine && mine->GetMinetype() == Mine::EMineType::Copper)
            {
                mine->Purchase();
                player->AddMineList(mine);
                break;
            }
        }
        return; // 함수 종료
    }

    long long savedGold = 0;
    long long savedTimeVal = 0;

    // 데이터 읽기 (골드 -> 시간)
    fscanf_s(file, "%lld", &savedGold);
    fscanf_s(file, "%lld", &savedTimeVal);

    // 골드 덮어쓰기
    player->SetGold(savedGold);
    
    for (Actor* actor : actors)
    {
        Mine* mine = actor->As<Mine>();

        if (mine != nullptr)
        {
            Mine::EMineType type = mine->GetMinetype();
            if (type == Mine::EMineType::None || type == Mine::EMineType::MaxCount)
            {
                continue;
            }

            int isPurchased = 0;
            int savedLevel = 1;

            // 파일에서 숫자 2개를 읽음 (구매여부, 레벨)
            fscanf_s(file, "%d %d", &isPurchased, &savedLevel);

            if (isPurchased == 1 && !mine->IsPurchased())
            {
                mine->Purchase();
                player->AddMineList(mine); // 보유목록에 등록
            }

            while (mine->GetLevel() < savedLevel)
            {
                mine->Upgrade();
            }
        }
    }

    fclose(file);
    // 현재 시간과 SaveData.txt에 적혀있는 시간
    time_t now = time(nullptr);
    time_t lastTime = (time_t)savedTimeVal;

    // 두 시간의 차이를 계산 초(double)단위로 반환
    double secondsPassed = difftime(now, lastTime);

    if (secondsPassed > 0)
    {
        long long totalOfflineIncome = 0;

        const std::vector<Mine*>& mines = player->GetOwnedMines();

        for (Mine* mine : mines)
        {
            float speed = mine->GetTimer().GetTargetTime();
            long long income = mine->GetIncome();
            
            if (speed > 0.0f)
            {
                long long count = (long long)(secondsPassed / speed);
                totalOfflineIncome += (count * income) / 10;
            }
        }

        // 수익 지급 및 로그 출력
        if (totalOfflineIncome > 0)
        {
            player->AddGold(totalOfflineIncome);

            char logBuffer[128];
            sprintf_s(logBuffer, "부재중 수익: %lld G (%.0f초)", totalOfflineIncome, secondsPassed);

            SetLog(logBuffer);
        }

    }
}
