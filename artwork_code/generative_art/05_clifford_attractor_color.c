#include "raylib.h"
#include <stdlib.h> 
#include <math.h>

#define WIDTH 1200
#define HEIGHT 1200
#define ITERATIONS 1000000

// RGB 값을 Color 구조체로 변환
Color RGB(unsigned char r, unsigned char g, unsigned char b) {
    return (Color){r, g, b, 255};
}

// HSV를 RGB로 변환 (0~1 범위)
Color HSVtoRGB(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h * 6, 2) - 1));
    float m = v - c;
    
    float r = 0, g = 0, b = 0;
    if (h < 1.0f/6) { r = c; g = x; b = 0; }
    else if (h < 2.0f/6) { r = x; g = c; b = 0; }
    else if (h < 3.0f/6) { r = 0; g = c; b = x; }
    else if (h < 4.0f/6) { r = 0; g = x; b = c; }
    else if (h < 5.0f/6) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }
    
    return RGB((unsigned char)((r+m)*255), 
               (unsigned char)((g+m)*255), 
               (unsigned char)((b+m)*255));
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, "Clifford Attractor - Generative Art");
    SetTargetFPS(60);

    Image image = GenImageColor(WIDTH, HEIGHT, RGB(220, 220, 225));
    Color *pixels = LoadImageColors(image);
    
    // 빈도 추적 배열 (선택적: 방문 횟수 기록)
    unsigned int *visitCount = (unsigned int *)malloc(WIDTH * HEIGHT * sizeof(unsigned int));
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        visitCount[i] = 0;
    }

    float a = -1.4f;
    float b = 1.5f;
    float c = 1.0f;
    float d = 0.9f;

    float x = 0.1f;
    float y = 0.1f;
    float scale = 280.0f;
    float offsetX = WIDTH / 2.0f;
    float offsetY = HEIGHT / 2.0f;

    // 속도 계산을 위한 이전 좌표
    float prev_x = x;
    float prev_y = y;

    for (int i = 0; i < ITERATIONS; i++) {
        prev_x = x;
        prev_y = y;
        
        float x_new = sinf(a * y) + c * cosf(a * x);
        float y_new = sinf(b * x) + d * cosf(b * y);
        
        x = x_new;
        y = y_new;

        // 속도 계산 (두 점 사이의 거리)
        float velocity = sqrtf((x - prev_x) * (x - prev_x) + 
                               (y - prev_y) * (y - prev_y));

        int px = (int)(x * scale) + (int)offsetX;
        int py = (int)(-y * scale) + (int)offsetY;

        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
            int index = py * WIDTH + px;
            visitCount[index]++;

// 방법 1: 좌표를 기반으로 Hue값을 계산
            float normalizedX = (x + 3.0f) / 6.0f;  // -3~3 범위를 0~1로 정규화
            float normalizedY = (y + 3.0f) / 6.0f;
            
            float hue = fmod(normalizedX + normalizedY, 1.0f);
            float saturation = 0.6f + 0.3f * fabs(sinf(normalizedX * 3.14159f));
            float brightness = 0.55f + 0.35f * fabs(cosf(normalizedY * 3.14159f));
            
            pixels[index] = HSVtoRGB(hue, saturation, brightness);

/* // 방법 2: 각도 기반 색상 변화
            float angle = atan2f(y, x) / 3.14159f;  // -1 ~ 1 범위
            float radius = sqrtf(x*x + y*y);
            
            float hue = fmod(angle * 0.5f + 0.5f, 1.0f);
            float saturation = 0.5f + 0.3f * fminf(radius / 2.0f, 1.0f);
            float brightness = 0.35f + 0.4f * fminf(radius / 2.0f, 1.0f);
            
            pixels[index] = HSVtoRGB(hue, saturation, brightness); */

/* --- 방법 1: 속도 기반 색상 ---
            // 속도를 0~1 범위로 정규화 (빠를수록 1에 가까움)
            float hue = fmod(velocity * 3.0f, 1.0f); // fmod로 값을 순환시켜 무지개 띠 느낌이 남
            float saturation = 0.8f + 0.2f * (velocity / 0.1f);  // 속도에 따라 채도 변화
            float brightness = 0.6f + 0.4f * fminf(velocity / 0.3f, 1.0f);
            
            pixels[index] = HSVtoRGB(hue, saturation, brightness); */

/* --- 방법 2: 좌표값 기반 색상 (대체 구현) ---
            // 좌표를 0~1 범위로 정규화 후 HSV로 변환
            float hue = fmod((x + 3.0f) / 6.0f, 1.0f);  // x 좌표 -> Hue
            float saturation = fmod((y + 3.0f) / 6.0f, 1.0f);  // y 좌표 -> Saturation
            float brightness = 0.7f;
            
            pixels[index] = HSVtoRGB(hue, saturation, brightness); */
        }
    }

    // 픽셀 데이터를 이미지로 복사
    unsigned char *data = (unsigned char *)image.data;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        data[i*4 + 0] = pixels[i].r;
        data[i*4 + 1] = pixels[i].g;
        data[i*4 + 2] = pixels[i].b;
        data[i*4 + 3] = pixels[i].a;
    }

    Texture2D texture = LoadTextureFromImage(image);
    
    UnloadImageColors(pixels);
    free(visitCount);
    UnloadImage(image);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RGB(220, 220, 225));
        DrawTexture(texture, 0, 0, RGB(255, 255, 255));
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();
    return 0;
}