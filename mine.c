
/*
 * mine.c
 *
 * Generates and dumps a minesweeper field to stdout, complete with number markers.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef char Tile;

typedef struct {
    int width;
    int height;
    Tile** tiles;
} Field;

Field* new_Field(int width, int height) {
    Field* field = malloc(sizeof(Field));
    field->width = width;
    field->height = height;
    field->tiles = (Tile**) malloc(sizeof(Tile*) * width);
    for (int i = 0; i < height; i++)
        field->tiles[i] = (Tile*) malloc(sizeof(Tile) * height);
    return field;
}

void destroy_Field(Field* field) {
    for (int i = 0; i < field->height; i++) {
        free(field->tiles[i]);
    }
    free(field->tiles);
    free(field);
}

void plantMines(Field* field) {
    srand(time(NULL));
    for (int x = 0; x < field->width; x++) {
        for (int y = 0; y < field->height; y++) {
            field->tiles[x][y] = ((rand() % 100) < 85) ? 0 : 9;
        }
    }
}

int inRange(Field* field, signed int x, signed int y) {
    return (
        x >= 0 &&
        x < (signed) field->width &&
        y >= 0 &&
        y < (signed) field->height) ? 1 : 0;
}

void markField(Field* field) {
    char mines;
    for (int x = 0; x < field->width; x++) {
        for (int y = 0; y < field->height; y++) {
            mines = 0;
            if (field->tiles[x][y] == 9) continue;
            for (signed int testX = x - 1; testX <= x + 1; testX++) {
                for (signed int testY = y - 1; testY <= y + 1; testY++) {
                    if (inRange(field, testX, testY)) {
                        if (field->tiles[testX][testY] == 9) {
                            mines++;
                        }
                    }
                }
            }
            field->tiles[x][y] = (Tile) mines;
        }
    }
}

void printUsage(void) {
    printf(
        "Usage: mine <width> <height>\n"
        "Width and height must be between 2 and 99.\n");
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage();
        return 1;
    }
    
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    
    printf("Input: width=%d, height=%d\n", width, height);
    
    if (width < 2 || width > 99 ||
        height < 2 || height > 99) {
        printUsage();
        return 1;
    }
    
    Field* field = new_Field(width, height);
    plantMines(field);
    markField(field);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (field->tiles[x][y] == 9)
                printf(" *");
            else
                printf(" %d", field->tiles[x][y]);
        }
        printf("\n");
    }
    
    destroy_Field(field);
    
    return 0;
}
