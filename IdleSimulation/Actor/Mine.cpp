#include "Mine.h"
#include "Math/Vector2.h"
#include "Render/Renderer.h"

// static 멤버 데이터 (규격 고정 : 가로 12, 세로 5)
const int MINE_WIDTH = 12;
const int MINE_HEIGHT = 5;

// static 멤버 변수 초기화 (클래스 밖에서)
const Mine::MineData Mine::mineInfos[] = {
    { EMineType::Copper, "구리", Color::Brown, 70000, 100, 20, 0.1f},
    { EMineType::Silver, "은",  Color::Gray, 300, 300, 50, 0.3f},
    { EMineType::Gold, "금",  Color::Yellow, 500, 1200, 500, 0.6f},
    { EMineType::Platinum, "백금", Color::White, 1500, 10000, 1000, 1.1f},
    { EMineType::Diamond, "다이아", Color::Cyan, 7000, 30000, 2000, 2.0f},
    { EMineType::Trophy, "전설의 트로피", Color::White, 0, 100000000, 0, 5.0f}
};

Mine::Mine(EMineType Minetype, Vector2 position, EventManager* eventManager)
    : super("", position, Color::White),
    mineType(Minetype),
    isPurchased(false),
    fillTimer(0.0f),
    blinkTimer(0.3f)
{
    // None(0)보다 크고 MaxCount(end)보다 작을 때만 데이터 접근
    if (Minetype > EMineType::None && Minetype < EMineType::MaxCount)
    {
        // mineInfos[0]에 접근하기 위해 -1 해줌
        int index = (int)Minetype - 1;
        mineData = &mineInfos[index];

        // 채굴Timer에 목표 시간 설정
        fillTimer.SetTargetTime(mineData->defaultFillSpeed);

        image = const_cast<char*>(mineData->name.c_str());
        color = mineData->colorCode;
        currentLevel = 1;
        currentIncome = mineData->basicIncome;
        currentUpgradePrice = mineData->upgradePrice;
    }
    else
    {
        // None 타입이거나 잘못된 타입일 때
        mineData = nullptr;
        image = const_cast<char*>(" ");
        currentLevel = 0;
        currentIncome = 0;
        currentUpgradePrice = 0;
    }


    if (eventManager != nullptr)
    {
        eventManager->Subscribe([this](EventManager::EEventType type)
            {
                switch (type)
                {
                case EventManager::EEventType::NONE:
                {
                    isPausedByEvent = false;
                    eventSpeedMultiplier = 1.0f;
                    break;
                }
                case EventManager::EEventType::EARTHQUAKE:
                {
                    isPausedByEvent = true;
                    break;
                }
                case EventManager::EEventType::GOLD_RUSH:
                {
                    isPausedByEvent = false;
                    eventSpeedMultiplier = 3.0f;
                    break;
                }

                }
            });
    }
    GeneratePath();
}

Mine::~Mine()
{
}

bool Mine::IsSelected(const Vector2& mousePos) const
{
    // 너비. 높이 절반
    int halfW = MINE_WIDTH / 2;
    int halfH = MINE_HEIGHT / 2;
    
    // 문자가 있는 위치(중심)
    int myPosX = (int)position.x;
    int myPosY = (int)position.y;

    // 마우스 위치
    int mouseX = (int)mousePos.x;
    int mouseY = (int)mousePos.y;

    bool inX = (mouseX >= myPosX - halfW) && (mouseX <= myPosX + halfW);
    bool inY = (mouseY >= myPosY - halfH) && (mouseY <= myPosY + halfH);
    return (inX && inY);
}

void Mine::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    if (mineType == EMineType::Trophy && !isPurchased)
    {
        blinkTimer.Tick(deltaTime);
        
        if (blinkTimer.IsTimeOut())
        {
            isBlinkWhite = !isBlinkWhite;
            blinkTimer.Reset();
        }
    }

    if (!isPurchased || borderPath.empty() || isPausedByEvent)
    {
        return;
    }
    
    // deltaTime에 eventSpeedMultiplier 곱해줌
    fillTimer.Tick(deltaTime * eventSpeedMultiplier);

    // 일정 시간마다 게이지 한 칸 채움
    if (fillTimer.IsTimeOut())
    {
        fillTimer.Reset();
        filledCount++;

        // 한 바퀴 다 돌기
        if (filledCount > borderPath.size())
        {
            filledCount = 0;
            // 수입 전달 (콜백 실행)
            if (onCycleComplete)
            {
                // income은 업그레이드 할 때 마다 상승함!
                onCycleComplete(currentIncome);
            }

        }
    }
}

void Mine::Draw()
{
    int halfW = MINE_WIDTH / 2;
    int halfH = MINE_HEIGHT / 2;

    Color eventBlinkColor = isBlinkWhite ? Color::Yellow : Color::White;

    // 테두리 그리기
    for (int i = 0; i < borderPath.size(); ++i)
    {
        Vector2 drawPos = position + borderPath[i];

        const char* symbol = "*";
        Color drawColor = Color::DarkGray;
        
        if (!isPurchased)
        {
            symbol = ".";
            drawColor = Color::Yellow;
            if (mineType == EMineType::Trophy)
            {
                symbol = "=";
                drawColor = Color::White;
            }
        }
        else
        {
            (i < filledCount) ? symbol = "o" : symbol = "*";
        
            if (isPausedByEvent)
            {
                drawColor = Color::Red;
            }
            else if (eventSpeedMultiplier > 1.0f)
            {
                drawColor = (i < filledCount) ? Color::Yellow : eventBlinkColor;
            }
            else
            {
                drawColor = (i < filledCount) ? Color::Green : Color::DarkGray;
            }   
        }

        Renderer::Get().Submit(symbol, drawPos, drawColor, 0);
    }
    if (mineType == EMineType::None || mineType == EMineType::MaxCount)
    {
        Renderer::Get().Submit("[ EMPTY ]", position + Vector2(-4, 0), Color::DarkGray, 1);
    }
    else if (isPurchased)
    {
        // 중심 심볼
        Color nameColor = mineData->colorCode;
        if (isPausedByEvent)
        {
            nameColor = Color::Red;
        }
        else if (eventSpeedMultiplier > 1.0f)
        {
            nameColor = eventBlinkColor;
        }

        Renderer::Get().Submit(image, position + Vector2(-1,0), nameColor, 1);

        // 레벨 표시 (심볼 바로위)
        std::string lvStr = "Lv." + std::to_string(currentLevel);
        Renderer::Get().Submit(lvStr, position + Vector2(-2, -1), Color::White, 2);
    }
    else if (mineType == EMineType::Trophy)
    {
        Color blinkColor = isBlinkWhite ? Color::White : Color::DarkRed;
      
        Renderer::Get().Submit(" PRESS ", position + Vector2(-4, -1), Color::DarkBlue, 6);
        Renderer::Get().Submit(" VICTORY ", position + Vector2(-4, 0), blinkColor, 7); // 여기에 적용!
    }
    else
    {
        Renderer::Get().Submit("?", position, Color::Yellow, -1);
        Renderer::Get().Submit("Locked", position + Vector2(-3, 1), Color::Gray, 2);
    }
    if (!mineData)
    {
        return;
    }

    // 가격 표시 로직 추가
    if (!isPurchased)
    {
        // 구매 전: 설치 비용 표시 (노란색)
        std::string price = "BUY: " + std::to_string(mineData->purchasePrice) + "G";
        if (mineData->purchasePrice < 10000) 
        {
            Renderer::Get().Submit(price, position + Vector2(-3, 3), Color::Yellow, 6);
        }
        else
        {
            Renderer::Get().Submit(price, position + Vector2(-6, 3), Color::Red, 6);
        }

    }
    else
    {
        // 구매 후: 다음 업그레이드 비용 표시 (흰색)
        if (mineType != EMineType::Trophy)
        {
            std::string upgrade = "UP: " + std::to_string(GetUpgradePrice()) + "G";
            Renderer::Get().Submit(upgrade, position + Vector2(-5, 3), Color::White, 6);
        }
    }
}

void Mine::Upgrade()
{
    currentLevel += 1;
    long long nextIncome = static_cast<long long>(currentIncome * 1.5f);
    nextIncome = (nextIncome <= currentIncome) ? currentIncome + 1 : nextIncome;
    currentIncome = nextIncome;
    currentUpgradePrice += static_cast<long long>(currentUpgradePrice * 2.1f);

    // 2번째 업그레이드에서는 광산의 채굴속도 향상
    if (currentLevel % 2 == 0)
    {
        // 현재 목표 시간을 가져와서 20% 단축
        float currentSpeed = fillTimer.GetTargetTime();
        if (currentSpeed > 0.02f)
        {
            currentSpeed *= 0.8f;
        }

        fillTimer.SetTargetTime(currentSpeed);
    }
}

void Mine::GeneratePath()
{
    borderPath.clear();
    int halfW = MINE_WIDTH / 2;
    int halfH = MINE_HEIGHT / 2;

    // 시계 방향으로 좌표 추가 (Top->Right->Bottom->Left)

    // 1. 위쪽 벽 (왼쪽 -> 오른쪽) x증가
    for (int x = -halfW; x < halfW; ++x)
    {
        // y값 고정, x값 가변
        borderPath.emplace_back(Vector2(x, -halfH));
    }

    // 2. 오른쪽 벽 (위-> 아래) y증가
    for (int y = -halfH; y < halfH; ++y)
    {
        // x값 고정, y값 가변
        borderPath.emplace_back(Vector2(halfW, y));
    }

    // 3. 아래쪽 벽 (오른쪽 -> 왼쪽) x감소
    for (int x = halfW; x > -halfW; --x)
    {
        borderPath.emplace_back(Vector2(x, halfH));
    }

    // 4. 왼쪽 벽 (아래 -> 위) y감소
    for (int y = halfH; y > -halfH; --y)
    {
        borderPath.emplace_back(Vector2(-halfW, y));
    }

 }
