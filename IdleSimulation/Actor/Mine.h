#pragma once
#include "Actor/Actor.h"
#include <string>
#include "Math/Color.h"
#include <vector>
#include <functional>

using namespace Wanted;

class Mine : public Actor
{
	RTTI_DECLARATIONS(Mine, Actor);

public:

    enum class EMineType
    {
        None = -1,
        Copper,
        Silver,
        Gold,
        Platinum,
        Diamond,
        MaxCount // 반복문 돌릴 때 유용함
    };

    struct MineData {
        EMineType type;           // 타입
        std::string name;         // 이름 ("구리")
        std::string symbol;       // 기호 
        Color colorCode;          // 색상 코드
        long long basicIncome;    // 기본 수입
        long long purchasePrice;  // 구입 비용
        long long upgradePrice;   // 업그레이드 비용
        int defaultRange; // 이 광물의 클릭 범위이자 크기
    };

    // 초기에 세팅된 값을 복사해옴
    static const MineData mineInfos[(int)EMineType::MaxCount];

public:
    Mine(EMineType type, Vector2 position);
	~Mine();
    const MineData& GetData() const { return *mineData; }

    bool IsSelected(const Vector2& mousePos) const;
    virtual void Tick(float deltaTime) override;
    virtual void Draw() override;

    // 외부에서 구입했을 때, 호출하는 함수
    void Purchase() { isPurchased = true; }
    bool IsPurchased() const { return isPurchased; }

    // 나중에 광산의 크기가 작아 질수도 커질수도 있음
    void SetRange(int newRange);

    // 광산을 업그레이드 했을 때, 데이터를 수정하는 함수
    void Upgrade();
    
    // 콜백 함수 타입 정의 (수입량을 인자로 받음)
    using OnCycleCompleteFuc = std::function<void(long long)>;

    // 콜백함수를 광산에 등록할 함수
    void SetOnCycleComplete(OnCycleCompleteFuc callback)
    {
        onCycleComplete = callback;
    }

    // 광산의 가변 데이터 Getter
    int GetLevel() const { return currentLevel; }
    long long GetUpgradePrice() const { return currentUpgradePrice; }
    long long GetIncome() const { return currentIncome; }

private:
    void GeneratePath();

private:	
    EMineType mineType;
    const MineData* mineData;

    // 현재 타이머 
    float currentTimer = 0.0f;

    // == 광산의 가변 데이터 ==
    // 현재 범위
    int currentRange;
    // 현재 광산의 업그레이드 가격
    long long currentUpgradePrice;
    // 현재 레벨 (업그레이드를 얼마나 눌렀는지 확인하는 변수)
    int currentLevel = 1;
    // 현재 수입
    long long currentIncome;
    
    // ???상태
    bool isPurchased = false;

    std::vector<Vector2> borderPath; // 테두리 좌표들

    int filledCount = 0;    // 현재 몇 칸이나 채워졌는지 (게이지)
    float fillTimer = 0.0f; // 채우기 타이머
    float fillSpeed = 0.1f; // 한 칸 채우는 시간

    OnCycleCompleteFuc onCycleComplete;
};

