# GEMINI.md - Wanted IdleGame 프로젝트 운영 원칙

이 프로젝트는 C++ 기반의 방치형 게임 엔진 및 시뮬레이션입니다. 모든 기여는 일관성과 기술적 무결성을 유지하기 위해 다음 지침을 반드시 준수해야 합니다.

## 1. 기술 스택 및 표준
- **언어:** C++17 이상.
- **네임스페이스:** 
    - 엔진 코드는 반드시 `Wanted` 네임스페이스 내에 위치해야 합니다.
    - 시뮬레이션 코드는 일관성을 위해 `Wanted` 네임스페이스 사용을 권장하지만, 기존 코드의 `using namespace Wanted;` 패턴이나 전역 네임스페이스 정의도 허용됩니다.
- **헤더 가드:** `#pragma once`를 독점적으로 사용합니다.
- **인코딩:** 한국어 주석을 올바르게 처리하기 위해 UTF-8 인코딩(MSVC의 경우 필요 시 BOM 포함)으로 저장해야 합니다.
- **AI 상호작용:** 모든 설명과 답변은 반드시 **한글**로 작성해야 합니다.

## 2. 명명 규칙 (Naming Conventions)
- **클래스 및 구조체:** PascalCase (예: `class Player`, `struct MineData`).
- **함수:** PascalCase (예: `void BeginPlay()`, `void Tick(float deltaTime)`).
- **멤버 변수:** camelCase (예: `long long gold`, `bool isPurchased`).
- **지역 변수:** camelCase (예: `float deltaTime`, `int index`).
- **매크로:** SCREAMING_SNAKE_CASE (예: `WANTED_API`, `RTTI_DECLARATIONS`).

## 3. 포맷팅 및 관례 (Formatting & Idioms)
- **들여쓰기:** 탭(Tab) 사용 (크기 4).
- **중괄호:** Allman 스타일 (새 줄에서 시작).
- **메모리 관리:** 수동 메모리 관리가 필요한 경우 `Engine/Util/Util.h`에 정의된 `SafeDelete(ptr)` 및 `SafeDeleteArray(ptr)`를 사용합니다.
- **수학:** 2D 좌표에는 `Vector2` (정수 기반)를 사용합니다.

## 4. 아키텍처 및 디자인 패턴
- **엔진 싱글톤:** `Engine` 클래스는 싱글톤 패턴을 따릅니다. `Engine::Get()`을 통해 접근하세요.
- **액터 생명주기:** `Actor`를 상속받는 클래스는 다음을 구현/재정의해야 합니다.
    - `BeginPlay()`: 초기 설정.
    - `Tick(float deltaTime)`: 프레임별 로직 업데이트.
    - `Draw()`: 렌더링 로직.
- **RTTI 시스템:** `RTTI`를 상속받는 모든 클래스(액터 포함)는 헤더에 `RTTI_DECLARATIONS(Type, ParentType)` 매크로를 포함해야 합니다.
- **DLL 관리:** `Engine` 프로젝트에서 내보낼(export) 클래스는 `WANTED_API`를 사용해야 합니다.

## 5. 파일 및 디렉터리 구조
- **Engine/**: 핵심 시스템 (Math, Render, Core, Actor, Level, Util).
- **IdleSimulation/**: 게임별 로직 및 액터 (Mine, Player, Managers).
- **Assets/**: 데이터 파일 (`Map.txt`, `Stage.txt`).
- **Config/**: 설정 및 저장 파일 (`Setting.txt`, `SaveData.txt`).
- **Includes/ & Library/**: 빌드 이벤트에 의해 관리되므로 수동으로 수정하지 마십시오.

## 6. 개발 및 빌드 워크플로우
- **빌드 시스템:** MSVC (Visual Studio 2022 이상).
- **구성:** x64 Debug/Release.
- **빌드 이벤트:** Engine 프로젝트에는 헤더를 `Includes/Engine`으로, 바이너리를 `Library/Engine`으로 복사하는 사전/사후 빌드 이벤트가 설정되어 있습니다. 해당 경로가 유효하고 쓰기 가능한지 확인하세요.
- **검증:** 코드 변경 후 프로젝트가 정상적으로 빌드되고 실행되는지 확인하십시오. 실행 시 `IdleSimulation`을 시작 프로젝트로 사용합니다.
- **문서화:** 기존의 주석 패턴을 유지하십시오.
