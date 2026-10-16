#include "raylib.h"
#include <math.h>

// 화면 해상도
#define WIDTH 1200
#define HEIGHT 1200
// 찍을 점의 개수 (클수록 디테일해지고 곡선이 부드러워짐)
#define ITERATIONS 700000

int main(void) {
    // 창 초기화
    InitWindow(WIDTH, HEIGHT, "Clifford Attractor - Generative Art");
    SetTargetFPS(60);

    // 1. 이미지 생성 및 메모리 포인터 가져오기
    Image image = GenImageColor(WIDTH, HEIGHT, (Color){ 20, 10, 30, 255 });
    // Color 포인터를 가져오면 배열처럼 [] 연산자로 픽셀에 직접 접근할 수 있습니다.
    Color *pixels = LoadImageColors(image); 

    // 2. 클리포드 어트랙터 파라미터 (a, b, c, d)
    // 이 4개의 숫자를 조금씩 바꿀 때마다 완전히 다른 패턴이 나옵니다!
    float a = -1.4f;
    float b = 1.5f;
    float c = 1.0f;
    float d = 0.9f;

    // 3. 초기 좌표 설정 (어디서 시작하든 결국 같은 패턴으로 수렴합니다)
    float x = 0.1f;
    float y = 0.1f;

    // 4. 수학적 좌표를 화면 픽셀 좌표로 매핑하기 위한 변수
    // 클리포드 어트랙터의 값은 대략 -3.0 ~ 3.0 사이를 벗어나지 않습니다.
    float scale = 250.0f; 
    float offsetX = WIDTH / 2.0f;
    float offsetY = HEIGHT / 2.0f;

    // 5. 핵심 루프: 점 찍기
    for (int i = 0; i < ITERATIONS; i++) {
        // 다음 좌표 계산
        float x_new = sinf(a * y) + c * cosf(a * x);
        float y_new = sinf(b * x) + d * cosf(b * y);
        
        x = x_new;
        y = y_new;

        // 수학 좌표 -> 화면 픽셀 좌표 변환 (y축이 반대임을 주의)
        int px = (int)(x * scale) + (int)offsetX;
        int py = (int)(-y * scale) + (int)offsetY; // -y를 곱해야 위로 향함

        // 배열 범위 검사 (화면 밖으로 벗어난 점은 무시)
        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
            int index = py * WIDTH + px; // 1차원 배열에서의 인덱스

            // 픽셀 색상 설정
            pixels[index] = (Color){ 110, 10, 160, 255 }; 
        }
    }

    // 6. 메모리에 그려진 픽셀 데이터를 다시 Image로 덮어씌움
    // (LoadImageColors로 가져온 포인터는 복사본이므로 다시 넣어줘야 함)
    unsigned char *data = (unsigned char *)image.data;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        data[i*4 + 0] = pixels[i].r; // R
        data[i*4 + 1] = pixels[i].g; // G
        data[i*4 + 2] = pixels[i].b; // B
        data[i*4 + 3] = pixels[i].a; // A
    }

    // 7. 완성된 Image를 Texture(그래픽 카드 메모리)로 업로드
    Texture2D texture = LoadTextureFromImage(image);

    // 메모리 해제 (이제 텍스처만 쓰면 되므로)
    UnloadImageColors(pixels);
    UnloadImage(image);

    // 메인 루프
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE); // 배경색 (실제로는 텍스처에 덮여서 보이지 않음)
        
        // Tint를 흰색으로 유지하면 텍스처가 원본 색상 그대로 표현됨
        DrawTexture(texture, 0, 0, (Color){255, 255, 255, 255});
        
        EndDrawing();
    }

    // 종료 시 메모리 해제
    UnloadTexture(texture);
    CloseWindow();

    return 0;
}