## I LOVE MONEY MINE

#### 프로젝트 개요
- 장르 : Console Game ( 방치형 광산 경영 시뮬레이션 게임)
- 개발 기간 : 5일 (2/5 ~ 2/6, 2/9 ~ 2/11)
- 핵심 컨셉 : 자원 채굴 및 업그레이드를 통해 수익을 극대화하는 방치형 시뮬레이션
- 참여 인원 : 1인
- 결과물 링크 : <https://blog.naver.com/hj10105/224181138135>
- 회고록 : <https://blog.naver.com/hj10105/224181135040>

#### 기술스택
- Language : C++
- Environment : Visual Studio 2022, Windows Console API
- Version Control : Git / GitHub
- AI : Gemini

#### 핵심 시스템 설계
1. 프로그래밍 언어 및 설계 (C++ & OOP)
- 커스텀 RTTI(Run-Time Type Information) 구현: * 표준 dynamic_cast 대신 클래스의 주소값과 매크로를 이용한 자체 타입 식별 시스템을 구축하여 객체 지향의 다형성을 안전하고 빠르게 활용함 (Actor::As<T>()).
- 디자인 패턴 활용:
  => Singleton Pattern: Engine, Renderer, Input 등 전역 시스템의 단일 객체 보장 및 접근성 확보.
  => Observer Pattern (Subscriber): EventManager를 통해 이벤트 발생 시 Mine이나 AdManager가 스스로 상태를 변경하도록 하는 느슨한 결합(Loose Coupling) 설계.
  => Delegate (Callback): std::function을 활용하여 광고 종료나 채굴 완료 시 특정 로직을 실행하는 이벤트 기반 프로그래밍 구현.

2. 게임 엔진 아키텍처 (Engine Architecture)
- 고정 프레임워크 기반 게임 루프: * QueryPerformanceCounter를 이용한 고정밀 타이머를 사용하여 프레임 독립적인 Tick(float deltaTime) 로직 구현.
- 더블 버퍼링 및 렌더링 파이프라인:
  => ScreenBuffer 클래스를 통한 이중 버퍼링 구현으로 콘솔 깜빡임(Flickering) 원천 차단.
  => Command Queue 방식의 렌더링: 모든 액터가 Submit을 통해 출력 정보를 예약하고, 렌더러가 일괄 처리하는 현대적 렌더링 파이프라인 모사.
  => Layer Sorting (Z-Order): sortingOrder를 도입하여 UI와 게임 오브젝트 간의 출력 우선순위 관리.

3. 게임 시스템 구현 능력 (Game Systems)
- 오프라인 수익 시스템 (Offline Profit):
  => time_t와 difftime 등 C++ 시간 라이브러리를 활용하여 현실 시간과 동기화된 보상 계산 로직 구현 (최대 6시간 제한 로직 포함).
- 비동기 입력 처리 시스템:
  => ReadConsoleInput API와 INPUT_RECORD를 활용하여 게임 루프 중단 없이 마우스 좌표 및 클릭 이벤트를 수집하는 Non-blocking Input 시스템 구축.
- 데이터 직렬화 (Serialization):
  => fopen_s, fprintf, fscanf_s를 이용해 플레이어 자산 및 광산별 레벨/구매 상태를 저장하고 불러오는 커스텀 세이브 시스템 구현.

4. 최적화 및 연출 (Optimization & UX)
- 화면 흔들림(Screen Shake) 알고리즘:
  => Renderer 내부에 shakeOffset 변수를 두어 게임 로직 수정 없이 전체 화면의 연출 효과를 제어하는 시스템 구현 (지진 이벤트).
- 절차적 테두리 생성 (Procedural Path):
  => 광산의 크기에 따라 GeneratePath 함수를 통해 채굴 게이지가 흐르는 좌표를 자동 계산하는 알고리즘 설계.
