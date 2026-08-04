#include <stdint.h>
#include "headers/cheats.h"

/* DEVIATION: cntlzw idiom; toggle deathless_player to its boolean inverse */
void cheat_f10(uint8_t key_is_down)
{
    if (key_is_down)
        cheat.deathless_player = (unsigned char)(__builtin_clz(cheat.deathless_player) & 0x20) != 0;
}
