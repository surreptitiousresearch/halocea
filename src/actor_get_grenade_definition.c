/* actor_get_grenade_definition @0x837B7F28 */
#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/game_globals.h"
#include "headers/cache_file_tag_instance.h"
#include "headers/game_globals_grenade.h"
#include "headers/blam_data_globals.h"

struct projectile_definition;


struct projectile_definition * actor_get_grenade_definition(int16_t grenade_type)
{
    /* game_globals.grenades.address slot (game_globals + 0x12C); each entry is a game_globals_grenade */
    game_globals_grenade *grenades_address = (game_globals_grenade *)global_game_globals->grenades.address;
    game_globals_grenade *grenade_entry = &grenades_address[grenade_type];

    if (grenade_entry)
    {
        int projectile_tag_index = grenade_entry->projectile.index;
        if (projectile_tag_index != -1)
            return TAG_GET(struct projectile_definition, projectile_tag_index);
    }
    return 0;
}
