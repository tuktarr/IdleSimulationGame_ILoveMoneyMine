#include "HelpManager.h"
#include "Core/Input.h"
#include "Render/Renderer.h"

HelpManager::HelpManager() : super()
{

}

void HelpManager::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    // H키나 ESC를 누르면 도움말 창 토글
    if (Input::Get().GetButtonDown('H'))
    {
        ToggleHelp();
    }
}

void HelpManager::Draw()
{
    super::Draw();
    if (!isVisible)
    {
        return;
    }
    int screenW = 110;
    int screenH = 28;

    // 전체 배경 테두리 그리기
    for (int y = 0; y < screenH; ++y)
    {
        for (int x = 0; x < screenW; ++x)
        {
            if (y == 0 || y == screenH - 1)
                Renderer::Get().Submit("-", Vector2(x, y), Color::Gray, 200);
            else if (x == 0 || x == screenW - 1)
                Renderer::Get().Submit("|", Vector2(x, y), Color::Gray, 200);
        }
    }
    const int UI_SORT = 1000;

    // 게임 타이틀 추가
    Renderer::Get().Submit(" ___  _      ___ __   __ ___   __  __  ___  _  _ ___ __   __  __  __ ___ _  _ ___ ", Vector2(12, 3), Color::Yellow, UI_SORT);
    Renderer::Get().Submit("|_ _| | |    / _ \\\\ \\ / // _ \\ |  \\/  |/ _ \\| \\| | __\\ \\ / / |  \\/  |_ _| \\| | __|", Vector2(12, 4), Color::Yellow, UI_SORT);
    Renderer::Get().Submit(" | |  | |__ | (_) |\\ V /|  __/ | |\\/| | (_) | .` | _| \\ V /  | |\\/| || || .` | _| ", Vector2(12, 5), Color::Yellow, UI_SORT);
    Renderer::Get().Submit("|___| |____| \\___/  \\_/  \\___| |_|  |_|\\___/|_|\\_|___| |_|   |_|  |_|___|_|\\_|___|", Vector2(12, 6), Color::Yellow, UI_SORT);

    // 조작법 섹션
    int labelX = 15;
    Renderer::Get().Submit("<<< GAME CONTROLS >>>", Vector2(labelX, 10), Color::Cyan, UI_SORT);
    Renderer::Get().Submit("- 마우스 좌클릭 : 광산 구매 / 업그레이드 선택", Vector2(labelX, 12), Color::White, UI_SORT);
    Renderer::Get().Submit("- [ AD ] 버튼   : 5초간 광고 시청 후 대량의 골드 획득", Vector2(labelX, 13), Color::White, UI_SORT);
    Renderer::Get().Submit("- [ H ] 키      : 도움말 화면 토글 (현재 화면)", Vector2(labelX, 14), Color::White, UI_SORT);
    Renderer::Get().Submit("- [ ESC ] 키    : 게임 종료 및 자동 저장", Vector2(labelX, 15), Color::White, UI_SORT);

    // 게임 시스템 섹션
    Renderer::Get().Submit("<<< SYSTEM INFO >>>", Vector2(labelX, 18), Color::Cyan, UI_SORT);
    Renderer::Get().Submit("- 오프라인 수익 : 게임 종료 후 최대 6시간까지 골드가 쌓입니다.", Vector2(labelX, 20), Color::White, UI_SORT);
    Renderer::Get().Submit("- 승리 조건     : 100억 G를 모아 '전설의 트로피'를 구매하세요.", Vector2(labelX, 21), Color::White, UI_SORT);
    Renderer::Get().Submit("- 속도 업그레이드 : 광산 레벨 2의 배수마다 채굴 속도가 20% 빨라집니다.", Vector2(labelX, 22), Color::White, UI_SORT);

    // 하단 안내 문구
    Renderer::Get().Submit("PRESS [ H ] TO RETURN TO MINING", Vector2(35, 25), Color::Green, UI_SORT);
}
