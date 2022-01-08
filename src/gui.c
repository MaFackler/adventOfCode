#include "common.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "extras/raygui.h"

#include "day20.c"


void draw(string_view sv) {
    int offset = 200;
    int x = 0;
    int y = 0;
    const int grid_size = 20;
    for (size_t i = 0; i < sv.size; ++i) {
        char c = sv.data[i];
        Color color = DARKGRAY;
        if (c == '\n') {
            y++;
            x = 0;
            continue;
        } else if (c == '#') {
            x++;
            color = DARKGRAY;
        } else if (c == '.') {
            x++;
            color = LIGHTGRAY;
        } else {
            assert(0);
        }
        DrawRectangle(offset + x * grid_size,
                      offset + y * grid_size,
                      grid_size,
                      grid_size,
                      color);
    }
}

int main() {
    const int width = 1024;
    const int height = 768;

    InitWindow(width, height, "Advent of Code 2021 Gui Playground");
    SetTargetFPS(60);
    
    char *buttons[] = {
        "Day 20",
    };

    string_view sv = {0};

    while (!WindowShouldClose()) {

        BeginDrawing();
            ClearBackground(RAYWHITE);
             
            int button_height = 50;
            for (size_t i = 0; i < ARRAY_LENGTH(buttons); ++i) {
                Rectangle button_size = {0, i * button_height, 100, button_height};
                if (GuiButton(button_size, buttons[i])) {
                    if (i == 0) {
                        sv = read_input();
                    }
                }
            }

            if (sv.size) {
                draw(sv);
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
