/* game_engine_get_variant @0x837482F0 — return the active multiplayer game variant. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_variant.h"


game_variant *game_engine_get_variant(void)
{
    return &global_variant;
}
