#include <pthread.h>
#include <unistd.h>
#include "game.h"
#include "clock.h"
#include "ioutils.h"

void *Tick(void *arg) {

    ClockArgs *args = (ClockArgs *)arg;

    Game *game = args->Game;

    pthread_mutex_t *renderMutex = args->RenderMutex;
    int renderFd = args->RenderFd;

    int moved;

    while (1) {

        sleep(1);
        moved = Game_MoveComb(game, game->Controlling, 0, 1);

        if (!moved) {
            // TODO: Check for landing, and combine blocks.
        }

        pthread_mutex_lock(renderMutex);
        WriteByte(renderFd, 't');
        pthread_mutex_unlock(renderMutex);

    }

}
