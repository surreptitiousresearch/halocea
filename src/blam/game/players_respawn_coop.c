/* players_respawn_coop @ 0x836AC7D0 — try to respawn dead co-op players at a living
 * teammate. Fails (with a reason code) if there are nearby dangers, attacking enemies, or
 * the only candidate is dead/in a vehicle. Returns 1 if at least one respawn happened.
 *
 * Raw object access reproduced from the decompiler: a player datum's unit index is at
 * +52 (dword 13); an object's data pointer is at +8 of its 12-byte header datum; the
 * object's position (real_point3d) is at +160 within the object data. */
#include <stdint.h>
#include "headers/players_global_data.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_datum_flags.h"
#include "headers/player_datum.h"
#include "headers/real_point3d.h"
#include "headers/player_respawn_failure.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t dangerous_projectiles_near_player(void);
extern uint8_t any_unit_is_dangerous(void);
extern uint8_t ai_enemies_attacking_player(void);
extern int object_get_ultimate_parent(int object_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void player_spawn(int player_index);
extern uint8_t player_teleport(int player_index, int source_unit_index, const real_point3d *position);

/* object data pointer for header index `idx` (header datum +8) */
static void *object_data(int idx)
{
    return ((void *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, idx)->datum);
}

uint8_t players_respawn_coop(void)
{
    int respawned = 0;
    data_iterator it;
    player_datum *player;
    int target_unit = -1;

    players_globals->respawn_failure_reason = _player_respawn_failure_none;

    if ( !players_globals->force_respawn
      && (dangerous_projectiles_near_player() || any_unit_is_dangerous()) )
    {
        players_globals->respawn_failure_reason = _player_respawn_failure_combat;
        goto done;
    }

    if ( !players_globals->force_respawn && ai_enemies_attacking_player() )
    {
        players_globals->respawn_failure_reason = _player_respawn_failure_enemies;
        goto done;
    }

    /* find a living, non-vehicle-trapped player unit to respawn others onto */
    data_iterator_new(&it, player_data);
    for ( player = data_iterator_next(&it); player; player = data_iterator_next(&it) )
    {
        int unit = player->unit_index;
        int in_bad_state = 0;
        int parent;
        if ( unit == -1 )
            continue;
        parent = object_get_ultimate_parent(unit);
        if ( parent == player->unit_index )
        {
            biped_datum *obj = object_try_and_get_and_verify_type(player->unit_index, object_mask_biped);
            if ( obj )
                in_bad_state = obj->biped.flags & (1u << _biped_airborne_bit);  /* DB biped_datum.flags bit0 = airborne */
        }
        else
        {
            vehicle_datum *vehicle = object_try_and_get_and_verify_type(parent, object_mask_vehicle);
            if ( vehicle )
                in_bad_state = vehicle->vehicle.airborne_ticks != 0;   /* +1232 */
        }
        if ( in_bad_state )
            players_globals->respawn_failure_reason = _player_respawn_failure_moving;
        else
            target_unit = player->unit_index;
    }

    if ( target_unit != -1 )
    {
        respawned = 1;
        data_iterator_new(&it, player_data);
        for ( player = data_iterator_next(&it); player; player = data_iterator_next(&it) )
        {
            if ( player->unit_index == -1 )
            {
                player_spawn(it.index);
                if ( player->unit_index == -1 )
                    respawned = 0;
                else
                    respawned = player_teleport(it.index, target_unit,
                        (const real_point3d *)((char *)object_data(target_unit) + 160));
            }
        }
    }

    players_globals->force_respawn = (players_globals->force_respawn && !respawned) ? 1 : 0;

done:
    if ( respawned )
        players_globals->respawn_failure_reason = _player_respawn_failure_none;
    return respawned;
}
