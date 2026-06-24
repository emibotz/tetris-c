#pragma once

#include <pthread.h>
#include "array.h"

typedef struct {
    int X;
    int Y;
} Block;

typedef struct {
    array_t(Block) Blocks;
    Block *Anchor;
} Combination;

typedef struct {
    pthread_rwlock_t RWLock;

    int Width;
    int Height;

    array_t(Combination *) Combinations;

    Combination *Controlling;
} Game;

Game *Game_New();

Block *Game_GetBlock(Game *game, int x, int y);
Block *Game_GetVisualBlock(Game *game, int x, int y);

int Game_MoveComb(Game *game, Combination *comb, int x, int y);

void Game_CheckControlling(Game *game);
