#pragma once

#include <pthread.h>
#include <time.h>
#include "game.h"

typedef struct {
    Game *Game;

    pthread_mutex_t *RenderMutex;
    int RenderFd;
} ClockArgs;

void *Tick(void *arg);
