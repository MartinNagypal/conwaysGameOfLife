#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const int size_x = 200;
const int size_y = 100;


const char *dead_output = "\u25A1";
const char *alive_output = "\u25A0";

enum cell_status {
    DEAD,
    ALIVE
};

int field[size_x][size_y];
int next_field[size_x][size_y];
int generation = 0;

void initField() {
    //set random seed
    srand(time(NULL));
    //init field with random values
    for (int x = 0; x<size_x; x++) {
        for (int y = 0; y<size_y; y++) {
            field[x][y] = (rand() % 4 == 0);
        }
    }
}

void print_field() {
    for (int y = 0; y<size_y; y++) {
        for (int x = 0; x<size_x; x++){
            if (field[x][y] == DEAD) printf("%s", dead_output);
            else printf("%s", alive_output);
        }
        printf("\n");
    }
}

void print_next_field() {
    for (int i = 0; i < size_x; i++) {
        printf("-");
    }
    printf("\n");
    printf("Generation: %d", generation);
    printf("\n");
    for (int y = 0; y<size_y; y++) {
        for (int x = 0; x<size_x; x++){
            if (next_field[x][y] == DEAD) printf("%s", dead_output);
            else printf("%s", alive_output);
        }
        printf("\n");
    }
    for (int i = 0; i < size_x; i++) {
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
        if (neighbour_cells[i][0] >= 0 && neighbour_cells[i][0] < size_x && neighbour_cells[i][1] >= 0 && neighbour_cells[i][1] < size_y) {
            //add field status (0 if dead, 1 if alive)
            count += field[neighbour_cells[i][0]][neighbour_cells[i][1]];
        }
    }
    return count;
}

void copy_field() {
    //copy new field into old one
    for (int y = 0; y<size_y; y++) {
        for (int x = 0; x < size_x; x++){
            field[x][y] = next_field[x][y];
        }
    }
}

void calc_new_generation() {
    //generation count
    generation++;
    //calc cell status
    for (int y = 0; y<size_y; y++) {
        for (int x = 0; x < size_x; x++){
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
    //print new field
    print_field();
}

int main(void) {
    initField();
    print_field();
    while (1) {
        printf("\n");
        calc_new_generation();
        print_next_field();
        getchar();
    }
    return 0;
}
