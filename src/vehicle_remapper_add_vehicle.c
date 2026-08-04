/* vehicle_remapper_add_vehicle @0x83700EE8 — register one scenario vehicle placement with the remapper. The
 * vehicle's definition reference index is matched against the per-type ref_index slots; if the active game
 * variant's engine accepts this placement (per a spawn-flag bit chosen by engine type), the placement is
 * appended to the matching [type][team] spawn list.
 *
 * Deviation: the decompiler walks ref_index with a raw pointer (stride 40 ints) and the per-struct end bound
 * label algn_8477A784; reproduced with an index loop over the 12 [type][team] slots. The flat team_vehicles[]
 * index (10 * slot + map_count) deliberately spans whole 80-byte structs, matching the original. */

#include <stdint.h>
#include "headers/vehicle_remapper_s.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"
#include "headers/multiplayer_spawn_game_type.h"

extern game_variant *game_engine_get_variant(void);

/* spawn_flags is stored via a halfword store (sthx) into the 16-bit team_vehicles[].spawn_flags field */
void vehicle_remapper_add_vehicle(unsigned int team, int vehicle_ref_index,
                                  unsigned int scenario_datum_index, uint16_t spawn_flags)
{
    /* locate the matching vehicle type by ref_index; the decompiler walks vehicles[type][0] (stride 2 structs) */
    unsigned int vehicle_type = 0;
    while ( vehicle_remapper.vehicles[vehicle_type][0].ref_index != (unsigned int)vehicle_ref_index )
    {
        if ( ++vehicle_type >= 6 )
            return;
    }

    game_variant *variant = game_engine_get_variant();
    unsigned char accepted;
    if ( variant )
    {
        if ( (unsigned int)(variant->game_engine_index - 1) > 3 )
            accepted = 1;
        else if ( variant->game_engine_index == game_engine_ctf )
            accepted = (spawn_flags >> (MULTIPLAYER_SPAWN_ACCEPT_SHIFT + _multiplayer_spawn_ctf_bit)) & 1;
        else if ( variant->game_engine_index == game_engine_slayer )
            accepted = (spawn_flags >> (MULTIPLAYER_SPAWN_ACCEPT_SHIFT + _multiplayer_spawn_slayer_bit)) & 1;
        else if ( variant->game_engine_index == game_engine_oddball )
            accepted = (spawn_flags >> (MULTIPLAYER_SPAWN_ACCEPT_SHIFT + _multiplayer_spawn_oddball_bit)) & 1;
        else /* king */
            accepted = (spawn_flags >> (MULTIPLAYER_SPAWN_ACCEPT_SHIFT + _multiplayer_spawn_king_bit)) & 1;
    }
    else
    {
        accepted = 0; /* uninitialized stack byte in the decompiler when no variant; treated as reject */
    }

    if ( accepted && vehicle_type <= 5 && scenario_datum_index != -1 )
    {
        if ( team >= 2 )
            team = 0;
        unsigned int flat_slot = 2 * vehicle_type + team;
        unsigned int map_count = vehicle_remapper.vehicles[0][flat_slot].map_count;
        vehicle_remapper.vehicles[0][flat_slot].map_count = map_count + 1;
        unsigned int entry = 10 * flat_slot + map_count;
        vehicle_remapper.vehicles[0][0].team_vehicles[entry].scenario_datum_index = scenario_datum_index;
        vehicle_remapper.vehicles[0][0].team_vehicles[entry].spawn_flags = spawn_flags;
    }
}
