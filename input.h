#pragma once

#include <pthread.h>
#include "game.h"

typedef struct {
    Game *Game;

    pthread_mutex_t *RenderMutex;
    int RenderFd;
} InputArgs;

void *Receive(void *arg);
