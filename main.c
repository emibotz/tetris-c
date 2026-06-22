#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/poll.h>
#include "array.h"
#include "game.h"
#include "clock.h"
#include "input.h"
#include "ioutils.h"

/*
 * Is it too much complexity for a tetris game?
 * I don't know...
 */

#define term_clear() printf("\033[H\033[J")
#define term_goto(x, y) printf("\033[%d;%dH", (x), (y))

void renderBlock(Game *game, int x, int y) {

    if (game == NULL) {
        return;
    }

    if (x == -1 || x == game->Width) {
        printf("|");
        return;
    }

    if (y == -1 || y == game->Height) {
        printf("－");
        return;
    }

    int i, j;
    int n = arr_count(game->Combinations);

    if (Game_GetVisualBlock(game, x, y)) {
        printf("⬛");
        return;
    }

    printf("\u3000"); // 全角空格

}

void render(Game *game) {
    int x, y, i;

    term_clear();
    term_goto(0, 0);

    for (y = -1; y < game->Height + 1; y += 1) {
        for (x = -1; x < game->Width + 1; x += 1) {
            renderBlock(game, x, y);
        }
        printf("\n");
    }
}

Combination *test1() {

    Combination *comb = malloc(sizeof(Combination));
    arr_init(comb->Blocks);

    Block block;

    block.X = 0;
    block.Y = 0;
    arr_push(comb->Blocks, block);

    block.X = 1;
    block.Y = 0;
    arr_push(comb->Blocks, block);

    block.X = 0;
    block.Y = 1;
    arr_push(comb->Blocks, block);

    block.X = 0;
    block.Y = 2;
    arr_push(comb->Blocks, block);

    return comb;

}

Combination *test2() {

    Combination *comb = malloc(sizeof(Combination));
    arr_init(comb->Blocks);

    Block block;
    int i;

    for (i = 0; i < 10; i += 1) {
        block.X = i;
        block.Y = 19;
        arr_push(comb->Blocks, block);
    }

    return comb;

}

int main() {

    // 禁用终端行缓冲区
    struct termios attr;
    tcgetattr(STDIN_FILENO, &attr);
    attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &attr);

    Game *game = (Game *)malloc(sizeof(Game));

    game->Width = 10;
    game->Height = 20;

    game->Controlling = test1();

    arr_init(game->Combinations);

    int renderPipes[2];
    int ok = pipe(renderPipes);

    if (ok == -1) {
        printf("failed to create pipe: %s\n", strerror(errno));
    }

    pthread_mutex_t renderMutex;
    pthread_mutex_init(&renderMutex, NULL);

    InputArgs inputArgs;

    inputArgs.Game = game;
    inputArgs.RenderMutex = &renderMutex;
    inputArgs.RenderFd = renderPipes[1];

    ClockArgs clockArgs;

    clockArgs.Game = game;
    clockArgs.RenderMutex = &renderMutex;
    clockArgs.RenderFd = renderPipes[1];

    pthread_t t;
    pthread_create(&t, NULL, Receive, &inputArgs);
    pthread_create(&t, NULL, Tick, &clockArgs);

    char input;

    int down;
    int moved;

    while (1) {
        int ok = ReadByte(renderPipes[0], &input);
        if (ok == -1) {
            printf("failed to read: %s\n", strerror(errno));
        }
        if (ok == 0) {
            goto endLoop;
        }

        render(game);

        switch (input) {
            case 'i': {
                ReadInt(renderPipes[0], &moved);
                ReadInt(renderPipes[0], &down);

                if (!moved && !down) {
                    printf("Blocked!!\n");
                }

                break;
            }
            case 't': {
                break;
            }
            default: {
                printf("wtf? received: %c\n", input);
                break;
            }
        }

    }
    endLoop:

    printf("stopped.\n");

}
