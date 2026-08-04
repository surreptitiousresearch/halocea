/* game_set_game_variant_from_name @0x83688628 — look up a multiplayer game variant by name and install it as
 * the active variant, then refresh the network server's view of it.
 *
 * DEVIATION: the decompiler emits a never-true stack-address guard around a memset of the global variant;
 * that branch is dead (the address compare can never hold), so only the live copy path is reproduced. */

#include <stdint.h>
#include "headers/game_globals.h"
#include <string.h>
#include "headers/blam_data_globals.h"

extern uint8_t game_engine_get_variant_by_name(const char *name, game_variant *variant);
extern network_game_server *global_network_game_server_get(void);

void game_set_game_variant_from_name(const char *name)
{
    game_variant variant;
    game_engine_get_variant_by_name(name, &variant);
    memcpy(&game_variant_global, &variant, sizeof(game_variant_global));
    global_network_game_server_get();
}
