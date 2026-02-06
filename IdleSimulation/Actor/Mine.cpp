#include "Mine.h"
#include "Math/Vector2.h"
#include "Render/Renderer.h"

// static 멤버 데이터 (규격 고정 : 가로 12, 세로 5)
const int MINE_WIDTH = 12;
const int MINE_HEIGHT = 5;

// static 멤버 변수 초기화 (클래스 밖에서)
const Mine::MineData Mine::mineInfos[] = {
    { EMineType::Copper, "구리", Color::Brown, 20, 100, 50,0.1f},
    { EMineType::Silver, "은",  Color::Gray, 50, 250, 250,0.3f},
    { EMineType::Gold, "금",  Color::Yellow, 200, 600, 1000,0.6f},
    { EMineType::Platinum, "백금", Color::White, 800, 10000, 5000,1.1f},
    { EMineType::Diamond, "다이아", Color::Cyan, 1500, 50000, 25000,2.0f}
};

Mine::Mine(EMineType type, Vector2 position)
    : Actor("", position, Color::White),
    mineType(type),
    isPurchased(false)
{
    // None(0)보다 크고 MaxCount(6)보다 작을 때만 데이터 접근
    if (type > EMineType::None && type < EMineType::MaxCount)
    {
        // mineInfos[0]에 접근하기 위해 -1 해줌
        int index = (int)type - 1;
        mineData = &mineInfos[index];

        fillSpeed = mineData->defaultFillSpeed;
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
        image = const_cast<char*>(" "); // 또는 [ ] 
        currentLevel = 0;
        currentIncome = 0;
        currentUpgradePrice = 0;
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
    if (!isPurchased || borderPath.empty())
    {
        return;
    }

    fillTimer += deltaTime;

    // 일정 시간마다 게이지 한 칸 채움
    if (fillTimer >= fillSpeed)
    {
        fillTimer = 0.0f;
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
    // Renderer 인스턴스 가져오기
    Renderer& renderer = Renderer::Get();
    int halfW = MINE_WIDTH / 2;
    int halfH = MINE_HEIGHT / 2;

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
        }
        else
        {
            if (i < filledCount)
            {
                symbol = "o";
                drawColor = Color::Green;
            }
            else
            {
                symbol = "*";
                drawColor = Color::DarkGray;
            }
        }
        renderer.Submit(symbol, drawPos, drawColor, 0);
    }
    if (mineType == EMineType::None || mineType == EMineType::MaxCount)
    {
        renderer.Submit("[ EMPTY ]", position + Vector2(-4, 0), Color::DarkGray, 1);
    }
    else if (isPurchased)
    {
        // 중심 심볼
        renderer.Submit(image, position + Vector2(-1,0), mineData->colorCode, 1);

        // 레벨 표시 (심볼 바로위)
        std::string lvStr = "Lv." + std::to_string(currentLevel);
        renderer.Submit(lvStr, position + Vector2(-2, -1), Color::White, 2);
    }
    else
    {
        renderer.Submit("?", position, Color::Yellow, -1);
        renderer.Submit("Locked", position + Vector2(-3, 1), Color::Gray, 2);
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
    if (currentLevel % 2 == 0 && fillSpeed > 0.02f)
    {
        fillSpeed *= 0.97f;
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
