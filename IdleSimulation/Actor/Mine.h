#pragma once
#include "Actor/Actor.h"
#include <string>
#include "Math/Color.h"
#include "Util/Timer.h"
#include "Actor/EventManager.h"
#include <vector>
#include <functional>


using namespace Wanted;

class Mine : public Actor
{
	RTTI_DECLARATIONS(Mine, Actor);

public:

    enum class EMineType
    {
        None = 0,
        Copper,
        Silver,
        Gold,
        Platinum,
        Diamond,
        Trophy, // 사면 승리하는 광산
        MaxCount // 반복문 돌릴 때 유용함
    };

    struct MineData {
        EMineType Minetype;           // 타입
        std::string name;         // 이름 ("구리")
        Color colorCode;          // 색상 코드
        long long basicIncome;    // 기본 수입
        long long purchasePrice;  // 구입 비용
        long long upgradePrice;   // 업그레이드 비용
        float defaultFillSpeed;    // 기본 채굴 속도
    };

    // 초기에 세팅된 값을 복사해옴
    static const MineData mineInfos[(int)EMineType::MaxCount];

public:
    Mine(EMineType type, Vector2 position, EventManager* eventManager = nullptr);
	~Mine();
    const MineData& GetData() const { return *mineData; }

    bool IsSelected(const Vector2& mousePos) const;
    virtual void Tick(float deltaTime) override;
    virtual void Draw() override;

    // 외부에서 구입했을 때, 호출하는 함수
    void Purchase() { isPurchased = true; }
    bool IsPurchased() const { return isPurchased; }

    // 광산을 업그레이드 했을 때, 데이터를 수정하는 함수
    void Upgrade();
    
    // 콜백 함수 타입 정의 (수입량을 인자로 받음)
    using OnCycleCompleteFuc = std::function<void(long long)>;

    // 콜백함수를 광산에 등록할 함수
    void SetOnCycleComplete(OnCycleCompleteFuc callback)
    {
        onCycleComplete = callback;
    }

    EMineType GetMinetype() { return mineType; }
    
    // 광산의 가변 데이터 Getter
    int GetLevel() const { return currentLevel; }
    long long GetUpgradePrice() const { return currentUpgradePrice; }
    long long GetIncome() const { return currentIncome; }
    Timer GetTimer() const { return fillTimer; }
private:
    void GeneratePath();

private:	
    EMineType mineType;
    const MineData* mineData;

    // 이벤트 상태 관리를 위한 변수
    float eventSpeedMultiplier = 1.0f; // 노다지 시 속도 배율 (기본 1.0)
    bool isPausedByEvent = false;      // 지진 시 일시정지 여부

    // == 광산의 가변 데이터 ==

    // 현재 광산의 업그레이드 가격
    long long currentUpgradePrice;
    // 현재 레벨 (업그레이드를 얼마나 눌렀는지 확인하는 변수)
    int currentLevel = 1;
    // 현재 수입
    long long currentIncome;
    
    // ???상태
    bool isPurchased = false;

    std::vector<Vector2> borderPath; // 테두리 좌표들

    int filledCount = 0; // 현재 몇 칸이나 채워졌는지 (게이지)
    Timer fillTimer;
    OnCycleCompleteFuc onCycleComplete;

    // 깜빡임을 저장할 변수
    Timer blinkTimer;
    bool isBlinkWhite = false;

};

