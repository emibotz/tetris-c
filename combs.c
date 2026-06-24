#include "combs.h"
#include "game.h"

Combination *Comb_Random() {
    Combination *comb = (Combination *)malloc(sizeof(Combination));
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

    block.X = 1;
    block.Y = 1;
    arr_push(comb->Blocks, block);

    return comb;
}
