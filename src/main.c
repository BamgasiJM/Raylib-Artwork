#include "raylib.h"

int main(void)
{
    InitWindow(1080, 1080, "Shader Test");

    Shader shader =
        LoadShader(
            "artwork_code/shader_art/01_shader_basic/shaders/vert.glsl",
            "artwork_code/shader_art/01_shader_basic/shaders/frag.glsl");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        BeginShaderMode(shader);

        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            WHITE);

        EndShaderMode();

        EndDrawing();
    }

    UnloadShader(shader);
    CloseWindow();

    return 0;
}