#include "Mine.h"

// static 멤버 변수 초기화 (클래스 밖에서)
const Mine::MineData Mine::mineInfos[] = {
    { EMineType::Copper,   "구리",   "Cu", Color::Black,   10,   100,   50  },
    { EMineType::Silver,   "은",     "Ag", Color::Gray,   50,   500,   250  },
    { EMineType::Gold,     "금",     "Au", Color::Yellow,  200,  2000,  1000 },
    { EMineType::Platinum, "백금",   "Pt", Color::White,  1000, 10000, 5000  },
    { EMineType::Diamond,  "다이아", "Di", Color::Cyan,  5000, 50000, 25000 }
};

Mine::Mine(EMineType type, Vector2 position)
    : Actor(
        mineInfos[(int)type].symbol.c_str(),
        position,
        mineInfos[(int)type].colorCode
    ),
    mineType(type),
    currentTimer(0.0f)
{
    if (type >= EMineType::Copper && type < EMineType::MaxCount)
    {
        mineData = &mineInfos[(int)type];
    }
}

Mine::~Mine()
{
}
