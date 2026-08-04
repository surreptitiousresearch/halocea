/* game_options_new @0x836881B8 — initialize a game_options block: zero it, then set default
 * difficulty (normal = 1) and a fixed default random seed (0xDEADBEEF).
 *
 * attested vs prior draft: disasm 0x836881E8/0x836881EC is two sth stores — code_version = 0,
 * difficulty = 1 — not one dword write of 1 at &code_version; the old pun produced identical
 * bytes only on big-endian. */

#include "headers/game_options.h"
#include <string.h>

void game_options_new(game_options *options)
{
    memset(options, 0, sizeof(game_options));
    options->code_version = 0; /* redundant after memset, but the binary stores it explicitly */
    options->difficulty = 1;
    options->random_seed = 0xDEADBEEF;
}
