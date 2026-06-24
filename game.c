#include "game.h"
#include "combs.h"
#include <unistd.h>

Block *Comb_GetBlock(Combination *comb, int x, int y) {
    if (comb == NULL) {
        return NULL;
    }

    int i;
    int n = arr_count(comb->Blocks);

    for (i = 0; i < n; i += 1) {
        Block *block = comb->Blocks + i;

        if (block->X == x && block->Y == y) {
            return block;
        }
    }

    return NULL;
}

Block *Game_getBlock(Game *game, int x, int y) {
    if (x < 0 || x >= game->Width || y < 0 || y >= game->Height) {
        return game->Controlling->Blocks;
    }

    int i;
    int n = arr_count(game->Combinations);

    for (i = 0; i < n; i += 1) {
        Combination *c = game->Combinations[i];

        Block *block = Comb_GetBlock(c, x, y);
        if (block != NULL) {
            return block;
        }
    }

    return NULL;
}

Block *Game_GetBlock(Game *game, int x, int y) {
    if (game == NULL) {
        return NULL;
    }

    pthread_rwlock_t *lock = &game->RWLock;
    pthread_rwlock_rdlock(lock);

    Block *block = Game_getBlock(game, x, y);

    pthread_rwlock_unlock(lock);
    return block;
}

Block *Game_GetVisualBlock(Game *game, int x, int y) {
    if (game == NULL) {
        return NULL;
    }

    Block *block = Game_GetBlock(game, x, y);
    if (block != NULL) {
        return block;
    }

    pthread_rwlock_t *lock = &game->RWLock;
    pthread_rwlock_rdlock(lock);

    block = Comb_GetBlock(game->Controlling, x, y);

    pthread_rwlock_unlock(lock);
    return block;
}

int Game_MoveComb(Game *game, Combination *comb, int x, int y) {
    if (comb == NULL || game == NULL) {
        return 0;
    }

    pthread_rwlock_t *lock = &game->RWLock;
    pthread_rwlock_wrlock(lock);

    int i;
    int n = arr_count(comb->Blocks);

    for (i = 0; i < n; i += 1) {
        Block *block = comb->Blocks + i;

        if (Game_getBlock(game, block->X + x, block->Y + y) != NULL) {
            pthread_rwlock_unlock(lock);
            return 0;
        }
    }

    for (i = 0; i < n; i += 1) {
        Block *block = comb->Blocks + i;
        block->X += x;
        block->Y += y;
    }

    pthread_rwlock_unlock(lock);
    return 1;
}

void Game_combine(Game *game, Combination *comb, int x, int y) {
    if (x < 0 || x >= game->Width || y < 0 || y >= game->Height) {
        return;
    }

    int i;
    int n = arr_count(game->Combinations);
    Combination *c = NULL;
    for (i = 0; i < n; i += 1) {
        c = game->Combinations[i];

        Block *block = Comb_GetBlock(c, x, y);
        if (block != NULL) {
            break;
        }
    }
    if (c == NULL || c == comb) {
        return;
    }
    arr_delete(game->Combinations, i - 1, i);
    n = arr_count(c->Blocks);

    for (i = 0; i < n; i += 1) {
        arr_push(comb->Blocks, c->Blocks[i]);
    }

    arr_free(c->Blocks);
    free(c);
}

// 这个函数会把所有在 game->Controlling 附近的方块组合中的
// 所有方块全都添加到自身列表中，然后用自己代替附近的方块
// 组合。
//
// 当然，如果方块超出屏幕，应该先结束游戏。
void Game_CheckControlling(Game *game) {
    if (game == NULL || game == NULL) {
        return;
    }

    pthread_rwlock_t *lock = &game->RWLock;
    pthread_rwlock_wrlock(lock);

    Combination *comb = game->Controlling;

    // TODO:
    // 在方块超出屏幕时，结束游戏！

    int i;
    int n = arr_count(comb->Blocks);

    for (i = 0; i < n; i += 1) {
        Block *block = comb->Blocks + i;
        Game_combine(game, comb, block->X, block->Y + 1);
    }

    arr_push(game->Combinations, game->Controlling);

    game->Controlling = Comb_Random();

    pthread_rwlock_unlock(lock);
}
