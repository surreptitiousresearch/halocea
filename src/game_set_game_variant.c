/* game_set_game_variant @0x83688310 — install the active game variant: copy it into the global slot
 * (touching the network game server), or clear the slot when given none. */

#include "headers/game_variant.h"
#include <string.h>
#include "headers/blam_data_globals.h"

extern network_game_server *global_network_game_server_get(void);

void game_set_game_variant(game_variant *variant)
{
    if ( variant )
    {
        memcpy(&game_variant_global, variant, sizeof(game_variant_global));
        global_network_game_server_get();
    }
    else
    {
        memset(&game_variant_global, 0, sizeof(game_variant_global));
    }
}
