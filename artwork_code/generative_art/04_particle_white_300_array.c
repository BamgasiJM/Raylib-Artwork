#include "raylib.h"      // raylib 그래픽 라이브러리 헤더 (창 생성, 그리기, 입력 처리 등)
#include <stdlib.h>      // 표준 라이브러리 (rand, srand 등 난수 생성 함수)
#include <time.h>        // 시간 관련 함수 (time()으로 시드 설정용)

// ===== 상수 정의 =====
#define SCREEN_WIDTH 1000      // 창의 가로 크기 (픽셀)
#define SCREEN_HEIGHT 1000     // 창의 세로 크기 (픽셀)
#define PARTICLE_COUNT 300     // 생성할 입자(파티클)의 총 개수

// ===== 입자 구조체 정의 =====
typedef struct
{
    Vector2 position;    // 입자의 현재 위치 (x, y 좌표) - raylib의 Vector2 타입 사용
    Vector2 velocity;    // 입자의 현재 속도 (x, y 방향 이동량) - Vector2 타입
    float radius;        // 입자의 반지름 (원의 크기)
} Particle;

// ===== 메인 함수 =====
int main(void)
{
    // ===== 윈도우 초기화 =====
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Array Particles");  // 창 생성 (너비, 높이, 제목)
    SetTargetFPS(60);    // 초당 60프레임으로 고정 (부드러운 애니메이션)

    // ===== 난수 시드 초기화 =====
    srand((unsigned int)time(NULL));   // 현재 시간을 기준으로 난수 시드 설정 (매번 다른 패턴)

    // ===== 입자 배열 선언 =====
    Particle particles[PARTICLE_COUNT];   // 300개의 입자를 저장할 배열

    // ===== 입자 초기화 (모든 입자에 초기값 설정) =====
    for (int i = 0; i < PARTICLE_COUNT; i++)
    {
        // 현재 입자 배열 요소 직접 접근 (포인터 대신 배열 사용)

        // ---- 반지름 설정 (3 ~ 5 픽셀 사이의 랜덤 값) ----
        particles[i].radius = (float)GetRandomValue(3, 5);

        // ---- 위치 설정 (창 경계에 닿지 않도록 반지름만큼 여유를 둠) ----
        particles[i].position.x =
                GetRandomValue(
                        (int)particles[i].radius,                      // 최소 x: 반지름만큼 왼쪽 여유
                        SCREEN_WIDTH - (int)particles[i].radius);      // 최대 x: 오른쪽 여유

        particles[i].position.y =
                GetRandomValue(
                        (int)particles[i].radius,                      // 최소 y: 반지름만큼 위쪽 여유
                        SCREEN_HEIGHT - (int)particles[i].radius);     // 최대 y: 아래쪽 여유

        // ---- 속도 설정 (-0.5 ~ 0.5 사이의 랜덤 실수) ----
        // GetRandomValue(-100, 100) 으로 -100~100 정수 생성 후 100으로 나누고 0.5를 곱함
        particles[i].velocity.x =
                ((float)GetRandomValue(-100, 100) / 100.0f) * 0.5f;

        particles[i].velocity.y =
                ((float)GetRandomValue(-100, 100) / 100.0f) * 0.5f;

        // ---- 속도가 0이면 강제로 방향 설정 (정지 방지) ----
        if (particles[i].velocity.x == 0.0f)
            particles[i].velocity.x = 0.3f;      // x 방향이 0이면 오른쪽으로 약간 이동

        if (particles[i].velocity.y == 0.0f)
            particles[i].velocity.y = -0.3f;     // y 방향이 0이면 위쪽으로 약간 이동
    }

    // ===== 메인 게임 루프 (창이 닫힐 때까지 반복) =====
    while (!WindowShouldClose())
    {
        // ===== 업데이트 단계 =====
        for (int i = 0; i < PARTICLE_COUNT; i++)
        {
            // 현재 입자 배열 요소 직접 접근 (포인터 대신 배열 사용)

            // ---- 위치 업데이트 (속도를 위치에 더함) ----
            particles[i].position.x += particles[i].velocity.x;
            particles[i].position.y += particles[i].velocity.y;

            // ---- 왼쪽 벽 충돌 처리 (반지름보다 작아지면 튕김) ----
            if (particles[i].position.x <= particles[i].radius)
            {
                particles[i].position.x = particles[i].radius;        // 벽에 붙도록 위치 보정
                particles[i].velocity.x *= -1.0f;           // x 속도 반전 (튕김)
            }

            // ---- 오른쪽 벽 충돌 처리 ----
            if (particles[i].position.x >= SCREEN_WIDTH - particles[i].radius)
            {
                particles[i].position.x = SCREEN_WIDTH - particles[i].radius;   // 오른쪽 벽에 붙도록 보정
                particles[i].velocity.x *= -1.0f;                     // x 속도 반전
            }

            // ---- 위쪽 벽 충돌 처리 ----
            if (particles[i].position.y <= particles[i].radius)
            {
                particles[i].position.y = particles[i].radius;         // 위쪽 벽에 붙도록 보정
                particles[i].velocity.y *= -1.0f;            // y 속도 반전
            }

            // ---- 아래쪽 벽 충돌 처리 ----
            if (particles[i].position.y >= SCREEN_HEIGHT - particles[i].radius)
            {
                particles[i].position.y = SCREEN_HEIGHT - particles[i].radius;   // 아래쪽 벽에 붙도록 보정
                particles[i].velocity.y *= -1.0f;                      // y 속도 반전
            }
        }

        // ===== 렌더링(그리기) 단계 =====
        BeginDrawing();                // 그리기 시작 (raylib 필수)
        ClearBackground(BLACK);        // 배경을 검은색으로 지움 (이전 프레임 제거)

        // ---- 모든 입자 그리기 ----
        for (int i = 0; i < PARTICLE_COUNT; i++)
        {
            // 현재 입자 배열 요소 직접 접근 (포인터 대신 배열 사용)

            // 흰색 원으로 입자를 그림 (위치, 반지름, 색상)
            DrawCircleV(
                    particles[i].position,     // 원의 중심 좌표 (Vector2)
                    particles[i].radius,       // 원의 반지름
                    WHITE);          // 색상 (흰색)
        }

        EndDrawing();    // 그리기 종료 (화면에 출력)
    }

    // ===== 윈도우 종료 및 리소스 해제 =====
    CloseWindow();    // raylib 윈도우 닫기
    return 0;         // 프로그램 정상 종료
}