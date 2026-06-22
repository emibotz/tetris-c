#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "input.h"
#include "ioutils.h"

void *Receive(void *arg) {

    InputArgs *args = (InputArgs *)arg;

    Game *game = args->Game;

    pthread_mutex_t *renderMutex = args->RenderMutex;
    int renderFd = args->RenderFd;

    int c;
    int down;
    int moved;

    while (1) {

        c = getchar();

        down = 0;
        moved = 0;

        switch (c) {
            case 'a': {
                moved = Game_MoveComb(game, game->Controlling, -1, 0);
                break;
                }

            case 'd': {
                moved = Game_MoveComb(game, game->Controlling, 1, 0);
                break;
                }

            case 's': {
                down = 1;
                do {
                    moved = Game_MoveComb(game, game->Controlling, 0, 1);
                } while(moved);
                break;
            }
        }

        pthread_mutex_lock(renderMutex);
        WriteByte(renderFd, 'i');
        WriteInt(renderFd, moved);
        WriteInt(renderFd, down);
        pthread_mutex_unlock(renderMutex);
        if (errno != 0) {
            printf("ahh %s", strerror(errno));
        }

    }

    return NULL;
}
