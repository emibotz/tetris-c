#include "game.h"

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
