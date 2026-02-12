#include "MenuLevel.h"
#include "GameLevel.h"
#include "Core/Input.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"

namespace Wanted
{
    MenuLevel::MenuLevel()
    {
        // 세이브 파일이 존재하는지 미리 체크
        FILE* file = nullptr;

        // 리턴값이 0이면 성공, 그 외에는 에러입니다.
        errno_t err = fopen_s(&file, "../Config/SaveData.txt", "r");

        if (err == 0 && file != nullptr)
        {
            bHasSaveFile = true;
            fclose(file);
        }
        else
        {
            bHasSaveFile = false;
        }
    }

    MenuLevel::~MenuLevel()
    {
    }

    void MenuLevel::Tick(float deltaTime)
    {
        super::Tick(deltaTime);
        HandleInput();
    }

    void MenuLevel::Draw()
    {
        super::Draw();

        // 타이틀 로고
        Renderer::Get().Submit(" ___  _      ___ __   __ ___   __  __  ___  _  _ ___ __   __  __  __ ___ _  _ ___ ", Vector2(15, 3), Color::Yellow, 0);
        Renderer::Get().Submit("|_ _| | |    / _ \\\\ \\ / // _ \\ |  \\/  |/ _ \\| \\| | __\\ \\ / / |  \\/  |_ _| \\| | __|", Vector2(15, 4), Color::Yellow, 0);
        Renderer::Get().Submit(" | |  | |__ | (_) |\\ V /|  __/ | |\\/| | (_) | .` | _| \\ V /  | |\\/| || || .` | _| ", Vector2(15, 5), Color::Yellow, 0);
        Renderer::Get().Submit("|___| |____| \\___/  \\_/  \\___| |_|  |_|\\___/|_|\\_|___| |_|   |_|  |_|___|_|\\_|___|", Vector2(15, 6), Color::Yellow, 0);
        Renderer::Get().Submit("========================= SIMULATOR =========================", Vector2(25, 8), Color::DarkGray, 0);

        // 마우스 위치 확인
        Vector2 mousePos = Input::Get().GetMousePosition();

        // 새 게임 버튼
        bool hoverNew = IsInside(mousePos, newGamePos, btnWidth, btnHeight);
        Renderer::Get().Submit("[  START NEW GAME  ]", newGamePos, hoverNew ? Color::White : Color::Green, 0);

        // 불러오기 버튼
        bool hoverLoad = IsInside(mousePos, loadGamePos, btnWidth, btnHeight);
        Color loadBtnColor;

        if (!bHasSaveFile)
        {
            loadBtnColor = Color::DarkGray; // 세이브 없으면 회색
        }
        else
        {
            loadBtnColor = hoverLoad ? Color::White : Color::Cyan;
        }

        std::string loadText = bHasSaveFile ? "[    LOAD GAME     ]" : "[ NO SAVE DATA  X ]";
        Renderer::Get().Submit(loadText, loadGamePos, loadBtnColor, 0);

        Renderer::Get().Submit("Use MOUSE L-BUTTON to Select", Vector2(42, 22), Color::Gray, 0);
    }

    void MenuLevel::HandleInput()
    {
        if (Input::Get().GetButtonDown(VK_LBUTTON))
        {
            Vector2 mPos = Input::Get().GetMousePosition();

            // 새 게임 클릭 시
            if (IsInside(mPos, newGamePos, btnWidth, btnHeight))
            {
                // 새 게임이므로 bShouldLoad = false 전달
                Engine::Get().SetNewLevel(new GameLevel(false));
            }
            // 불러오기 클릭 시 (세이브 파일이 있을 때만)
            else if (bHasSaveFile && IsInside(mPos, loadGamePos, btnWidth, btnHeight))
            {
                // 불러오기이므로 bShouldLoad = true 전달
                Engine::Get().SetNewLevel(new GameLevel(true));
            }
        }
    }

    bool MenuLevel::IsInside(Vector2 mousePos, Vector2 btnPos, int width, int height)
    {
        return (mousePos.x >= btnPos.x && mousePos.x <= btnPos.x + width &&
            mousePos.y >= btnPos.y && mousePos.y <= btnPos.y + height);
    }
}