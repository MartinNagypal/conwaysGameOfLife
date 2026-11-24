#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

int BLOCK_SIZE = 3;
const int WINDOW_X = 1800;
const int WINDOW_Y =900;
const int SIZE_X = 2000;
const int SIZE_Y = 2000;

const char *dead_output = "\u25A1";
const char *alive_output = "\u25A0";

clock_t current, last;
float speed = 250;
float max_delay = 600;
float min_delay = 0;

int autoCalc = 1;

enum cell_status {
    DEAD,
    ALIVE
};

int field[SIZE_X][SIZE_Y];
int next_field[SIZE_X][SIZE_Y];
int generation = 0;

void initField() {
    //set random seed
    srand(time(NULL));
    //init field with random values
    for (int x = 0; x<SIZE_X; x++) {
        for (int y = 0; y<SIZE_Y; y++) {
            field[x][y] = (rand() % 14 == 0);
        }
    }
}

void print_field() {
    for (int y = 0; y<SIZE_Y; y++) {
        for (int x = 0; x<SIZE_X; x++){
            if (field[x][y] == DEAD) printf("%s", dead_output);
            else printf("%s", alive_output);
        }
        printf("\n");
    }
}

void print_next_field() {
    for (int i = 0; i < SIZE_X; i++) {
        printf("-");
    }
    printf("\n");
    printf("Generation: %d", generation);
    printf("\n");
    for (int y = 0; y<SIZE_Y; y++) {
        for (int x = 0; x<SIZE_X; x++){
            if (next_field[x][y] == DEAD) printf("%s", dead_output);
            else printf("%s", alive_output);
        }
        printf("\n");
    }
    for (int i = 0; i < SIZE_X; i++) {
        printf("-");
    }
    printf("\n");
}

int count_neighbours(int x, int y) {
    //set coordinated of neighbours
    int neighbour_cells[8][2] = {{x-1, y-1}, {x, y-1}, {x+1, y-1}, {x-1, y}, {x+1, y}, {x-1, y+1}, {x, y+1}, {x+1, y+1}};
    int count = 0;
    for (int i = 0; i < 8; i++) {
        //check if index is in array
        if (neighbour_cells[i][0] >= 0 && neighbour_cells[i][0] < SIZE_X && neighbour_cells[i][1] >= 0 && neighbour_cells[i][1] < SIZE_Y) {
            //add field status (0 if dead, 1 if alive)
            count += field[neighbour_cells[i][0]][neighbour_cells[i][1]];
        }
    }
    return count;
}

void copy_field() {
    //copy new field into old one
    for (int y = 0; y<SIZE_Y; y++) {
        for (int x = 0; x < SIZE_X; x++){
            field[x][y] = next_field[x][y];
        }
    }
}

void calc_new_generation() {
    //generation count
    generation++;
    //calc cell status
    for (int y = 0; y<SIZE_Y; y++) {
        for (int x = 0; x < SIZE_X; x++){
            int neighbours = count_neighbours(x,y);
            if (field[x][y] == ALIVE && (neighbours < 2 || neighbours > 3)) {
                next_field[x][y] = DEAD;
            }
            else if (field[x][y] == DEAD && neighbours == 3) {
                next_field[x][y] = ALIVE;
            }
            else {
                next_field[x][y] = field[x][y];
            }
        }
    }
    //copy new field on old field
    copy_field();
}

void drawField() {
    ClearBackground(RAYWHITE);
    const int startX = 10, startY = 50;
    const int w = SIZE_X * BLOCK_SIZE, h = SIZE_Y * BLOCK_SIZE;

    DrawRectangle(startX, startY, w, h, WHITE);

    for (int y = 0; y < SIZE_Y; y++)
        for (int x = 0; x < SIZE_X; x++)
            if (field[y][x])
                DrawRectangle(startX + x*BLOCK_SIZE, startY + y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);

    for (int x = 0; x <= SIZE_X; x++)
        DrawLine(startX + x*BLOCK_SIZE, startY, startX + x*BLOCK_SIZE, startY + h, LIGHTGRAY);
    for (int y = 0; y <= SIZE_Y; y++)
        DrawLine(startX, startY + y*BLOCK_SIZE, startX + w, startY + y*BLOCK_SIZE, LIGHTGRAY);
}

void drawStats() {
    DrawText(TextFormat("Generation: %d", generation), 10, 10, 25, BLACK);
}

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void toggleAutoCalc() {
    autoCalc = (++autoCalc) %2;
}

void plus_size(int val) {
    BLOCK_SIZE = BLOCK_SIZE + val;
    if (BLOCK_SIZE > 30) {
        BLOCK_SIZE = 30;
    }
}
void minus_size(int val) {
    BLOCK_SIZE = BLOCK_SIZE - val;
    if (BLOCK_SIZE < 2) {
        BLOCK_SIZE = 2;
    }
}

void drawButtons() {
    if (GuiButton((Rectangle){300, 10, 90, 30}, "Auto")) {
        toggleAutoCalc();
    }
    if (GuiButton((Rectangle){400, 10, 90, 30}, "Step")) {
        calc_new_generation();
        drawField();
    }
    if (GuiButton((Rectangle){500, 10, 40, 30}, "Size +")) {
        plus_size(2);
    }
    if (GuiButton((Rectangle){550, 10, 40, 30}, "Size -")) {
        minus_size(2);
    }
    if (GuiButton((Rectangle){600, 10, 90, 30}, "Reactivate")) {
        generation = 0;
        initField();
    }
    GuiSliderBar((Rectangle){800, 10, 90, 30}, "Speed -", "Speed +", &speed, min_delay, max_delay);
}

int main(void) {
    initField();
    last = clock();
    InitWindow(WINDOW_X, WINDOW_Y, "Conway's Game of Life");
    SetWindowMinSize(WINDOW_X, WINDOW_Y);
    while(!WindowShouldClose()) {
        BeginDrawing();
        drawButtons();
        current = clock();
        if (autoCalc == 1) {
            double time_elapsed = ((double)(current-last) / CLOCKS_PER_SEC) * 1000.0;
            if (time_elapsed > (max_delay-speed)) {
                calc_new_generation();
                 last = clock();
            }
        }
        drawStats();
        drawField();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
