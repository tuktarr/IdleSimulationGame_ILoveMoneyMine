#include "Mine.h"
#include "Math/Vector2.h"
#include "Render/Renderer.h"

// static 멤버 변수 초기화 (클래스 밖에서)
const Mine::MineData Mine::mineInfos[] = {
    { EMineType::Copper, "구리", "C", Color::Brown, 10, 100, 50, 3},
    { EMineType::Silver, "은", "S", Color::Gray, 50, 500, 250, 2},
    { EMineType::Gold, "금", "G", Color::Yellow, 200, 2000, 1000, 2},
    { EMineType::Platinum, "백금","P", Color::White, 1000, 10000, 5000, 1},
    { EMineType::Diamond, "다이아","D", Color::Cyan, 5000, 50000, 25000, 1}
};

Mine::Mine(EMineType type, Vector2 position)
    : Actor(
        mineInfos[(int)type].symbol.c_str(),
        position,
        Color::White
    ),
    mineType(type),
    currentTimer(0.0f),
    isPurchased((false)),
    filledCount(0),
    fillTimer(0.0f),
    fillSpeed(0.1f)

{
    if (type >= EMineType::Copper && type < EMineType::MaxCount)
    {
        mineData = &mineInfos[(int)type];
        currentRange = mineData->defaultRange;
        // 구입 후의 광산 이미지 설정
        color = mineData->colorCode;

        // 가변 데이터 초기화 추가
        currentLevel = 1;
        currentIncome = mineData->basicIncome;
        currentUpgradePrice = mineData->upgradePrice;
    }
    else
    {
        currentRange = 1;
        currentLevel = 1;
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
    // 문자가 있는 위치(중심)
    int myPosX = (int)position.x;
    int myPosY = (int)position.y;

    // 마우스 위치
    int mouseX = (int)mousePos.x;
    int mouseY = (int)mousePos.y;

    bool inX = (mouseX >= myPosX - currentRange) && (mouseX <= myPosX + currentRange);
    bool inY = (mouseY >= myPosY - currentRange) && (mouseY <= myPosY + currentRange);
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

    // 테두리 그리기
    for (int i = 0; i < borderPath.size(); ++i)
    {
        Vector2 drawPos = position + borderPath[i];

        const char* symbol = "*";
        Color drawColor = Color::DarkGray;

        if (!isPurchased)
        {
            symbol = "?";
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

    // 본체 그리기
    Color centerColor = isPurchased ? mineData->colorCode : Color::White;
    renderer.Submit(image, position, centerColor, 1);
}
void Mine::SetRange(int newRange)
{
    currentRange = newRange;
    GeneratePath(); // 범위가 바뀌면 경로도 다시 계산
}

void Mine::Upgrade()
{
    currentLevel += 1;
    long long nextIncome = static_cast<long long>(currentIncome * 1.5f);
    nextIncome = (nextIncome <= currentIncome) ? currentIncome + 1 : nextIncome;
    currentIncome = nextIncome;
    currentUpgradePrice += static_cast<long long>(currentUpgradePrice * 2.1f);

    // 10번째 업그레이드에서는 광산의 채굴속도 향상
    if (currentLevel % 10 == 0 && fillSpeed > 0.02f)
    {
        fillSpeed -= 0.01f;
    }
}

void Mine::GeneratePath()
{
    borderPath.clear();
    filledCount = 0; // 경로가 바뀌면 게이지도 초기화

    if (currentRange <= 0)
    {
        return;
    }

    // 시계 방향으로 좌표 추가 (Top->Right->Bottom->Left)

    // 1. 위쪽 벽 (왼쪽 -> 오른쪽) x증가
    for (int x = -currentRange; x < currentRange; ++x)
    {
        borderPath.emplace_back(Vector2(static_cast<float>(x), static_cast<float>(-currentRange)));
    }

    // 2. 오른쪽 벽 (위-> 아래) y증가
    for (int y = -currentRange; y < currentRange; ++y)
    {
        borderPath.emplace_back(Vector2(static_cast<float>(currentRange), static_cast<float>(y)));
    }

    // 3. 아래쪽 벽 (오른쪽 -> 왼쪽) x감소
    for (int x = currentRange; x > -currentRange; --x)
    {
        borderPath.emplace_back(Vector2(static_cast<float>(x), static_cast<float>(currentRange)));
    }

    // 4. 왼쪽 벽 (아래 -> 위) y감소
    for (int y = currentRange; y > -currentRange; --y)
    {
        borderPath.emplace_back(Vector2(static_cast<float>(-currentRange), static_cast<float>(y)));
    }

 }
