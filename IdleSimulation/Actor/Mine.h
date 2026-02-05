#pragma once
#include "Actor/Actor.h"
#include <string>
#include "Math/Color.h"

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
        std::string symbol;       // 기호 ("Cu", "Ag" 등) - char* 대신 string 추천
        Color colorCode;            // 색상 코드 (구조체에 이거 추가해야 함!)
        long long basicIncome;    // 기본 수입
        long long purchasePrice;  // 구입 비용
        long long upgradePrice;   // 업그레이드 비용
    };

    static const MineData mineInfos[(int)EMineType::MaxCount];

public:
    Mine(EMineType type, Vector2 position);
	~Mine();

private:	
    EMineType mineType;
    const MineData* mineData;

    // 현재 타이머 
    float currentTimer = 0.0f;
};

