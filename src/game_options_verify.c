/* game_options_verify @0x83688208 — validate the difficulty field of a game_options block. Valid
 * difficulties are 0..3; anything else is rejected. */

#include "headers/game_options.h"

int game_options_verify(game_options *options)
{
    __int16 difficulty = options->difficulty;

    if ( difficulty < 0 )
        return 0;
    if ( difficulty >= 4 )
        return 0;
    return 1;
}
