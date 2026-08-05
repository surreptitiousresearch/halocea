/* unit_record_damage @0x836C89D8 — accumulate a damage event into the victim unit's four "recent damage"
 * slots (unit.attackers, used for assist/credit tracking), then, on a killing blow against an enemy, advance
 * the killer's kill-streak ("killing spree") counter.
 *
 * A slot matches when its player_index field equals owner_player_index (r7) or its object_index equals
 * owner_object_index (r9). With no match and no free (game_time_stamp == -1) slot, the slot evicted is the
 * oldest one that is NOT the current highest-damage slot. (owner_team_index (r8) is used only for the
 * kill-credit enemy test, not for slot matching — disasm 0x836C8A34/0x836C8BC4.)
 *
 * ABI verified from prologue (0x836C89D8) and call site (0x836D5DA8): 7 args; `damage` is a single float
 * reserving r4. The decompiler invents a trailing `a8` that is just a second (int) view of owner_object_index
 * (r9); its `died` param is the kill flag (r6), distinct from `damage_category` (r5) which drives the ==9
 * test. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"
#include "headers/damage_category.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t ai_handle_killing_spree(int unit_index, int16_t killing_spree_count);

void unit_record_damage(int unit_index, float damage, int16_t damage_category, uint8_t died,
                        int owner_player_index, int16_t owner_team_index, int owner_object_index)
{
    uint8_t found = 0;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int now = game_time_get();

    for ( int i = 0; i < 4; i = (int16_t)(i + 1) )
    {
        unit_attacker *slot = &unit->unit.attackers[i];
        if ( owner_player_index == -1 || slot->player_index != owner_player_index )
        {
            if ( slot->object_index != owner_object_index )
                continue;
        }
        slot->damage_inflicted += damage;
        found = 1;
        slot->game_time_stamp = now;
    }

    if ( !found )
    {
        int16_t evict = -1;
        int free_slot = 0;
        while ( unit->unit.attackers[free_slot].game_time_stamp != -1 )
        {
            free_slot = (int16_t)(free_slot + 1);
            if ( free_slot >= 4 )
                goto have_slot;
        }
        evict = free_slot;
    have_slot:
        if ( evict == -1 )
        {
            /* keep the highest-damage slot, evict the oldest of the rest */
            int16_t highest_damage = 0;
            for ( int i = 1; i < 4; i = (int16_t)(i + 1) )
                if ( unit->unit.attackers[i].damage_inflicted > unit->unit.attackers[highest_damage].damage_inflicted )
                    highest_damage = i;

            evict = -1;
            for ( int i = 0; i < 4; i = (int16_t)(i + 1) )
                if ( i != highest_damage
                  && (evict == -1
                      || unit->unit.attackers[i].game_time_stamp < unit->unit.attackers[evict].game_time_stamp) )
                    evict = i;
        }

        unit_attacker *slot = &unit->unit.attackers[evict];
        slot->game_time_stamp = now;
        slot->damage_inflicted = damage;
        slot->object_index = owner_object_index;
        slot->player_index = owner_player_index;
    }

    if ( died && owner_team_index != -1
      && game_team_is_enemy(unit->object.owner_team_index, owner_team_index) )
    {
        int killer_unit_index;
        unit_datum *killer = 0;
        /* resolve the killer unit: prefer the owning player's unit, else the owning object directly */
        if ( owner_player_index != -1 )
        {
            player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, owner_player_index);
            int player_unit_index = player->unit_index;
            if ( player_unit_index != -1 )
                killer = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                          player_unit_index)->datum;
        }
        killer_unit_index = owner_object_index;
        if ( !killer )
        {
            killer = object_try_and_get_and_verify_type(owner_object_index, object_mask_unit);
            if ( !killer )
                return;
        }

        /* step through the operator/parent chain: vehicle-category damage credits the driver, else the gunner */
        int responsible = damage_category == _damage_category_vehicle ? killer->unit.driver_object_index
                                               : killer->unit.gunner_object_index;
        if ( responsible != -1 )
        {
            killer_unit_index = responsible;
            killer = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, responsible)->datum;
        }

        if ( (killer->object.damage_flags & (1u << _object_dead_bit)) == 0 )  /* killer not itself dead */
        {
            int time = game_time_get();
            int last_kill_time = killer->unit.killing_spree_last_time;
            if ( last_kill_time == -1 || last_kill_time + 120 < time )
                killer->unit.killing_spree_count = 0;  /* streak timed out */
            int16_t spree = killer->unit.killing_spree_count;
            killer->unit.killing_spree_last_time = time;
            killer->unit.killing_spree_count = spree + 1;
            if ( ai_handle_killing_spree(killer_unit_index, spree + 1) )
                killer->unit.killing_spree_count = 0;
        }
    }
}
