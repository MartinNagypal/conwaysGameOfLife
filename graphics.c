//
// Created by Martin Nagypal on 23.11.26.
//
#include "raylib.h"

#define SIZE_X 25
#define SIZE_Y 15
const int BLOCK_SIZE = 30;

int main(void) {
    InitWindow(800, 600, "Game of Life");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        int startX = 10; int startY = 10;
        for (int y = 0; y < SIZE_Y; y++) {
            for (int x = 0; x < SIZE_X; x++) {
                if (x % 3 == 0) {
                    DrawRectangle((startX+(x*BLOCK_SIZE)), (startY+(y*BLOCK_SIZE)), BLOCK_SIZE, BLOCK_SIZE, WHITE);
                    DrawRectangleLines((startX+(x*BLOCK_SIZE)), (startY+(y*BLOCK_SIZE)), BLOCK_SIZE, BLOCK_SIZE, LIGHTGRAY);
                }
                else {
                    DrawRectangle((startX+(x*BLOCK_SIZE)), (startY+(y*BLOCK_SIZE)), BLOCK_SIZE, BLOCK_SIZE, BLACK);
                    DrawRectangleLines((startX+(x*BLOCK_SIZE)), (startY+(y*BLOCK_SIZE)), BLOCK_SIZE, BLOCK_SIZE, LIGHTGRAY);
                }
            }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}