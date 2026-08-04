/* prop_add @0x837D1768 — initialize a freshly allocated prop and prepend it to the actor's prop list. Clears
 * the prop's tracking/perception fields to their "none" sentinels, then (when the unit exists) fills in the
 * team relationship (enemy/ally/status-changed vs the actor's team), dead/player flags, suicide radius (the
 * unit tag's ai_danger_radius), and the unit's controlling actor (swarm controller preferred). The prop's
 * dead_ticks seed is 1000 when the unit is already dead. 2026-07-13: fully typed (prop_datum/unit_datum);
 * the raw-offset transcription's "has_special_flag"/"suppressed" were the dead / really-dead
 * (dead-and-not-feigning) pair, as in prop_status_refresh.c. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern uint8_t game_team_is_ally(int16_t our_team, int16_t other_team);
extern uint8_t game_team_ally_status_changed(int16_t our_team_index, int16_t other_team_index);
extern int game_time_get(void);

void prop_add(int actor_index, int unit_index, int prop_index)
{
    if ( prop_index == -1 )
        return;

    prop_datum *prop = DATUM_GET(prop_data, prop_datum, prop_index);

    prop->damage_inflicted_on_me = 0.0f;
    prop->owner_actor_index = actor_index;
    prop->unit_index = unit_index;
    prop->unit_effect = -1;
    prop->ticks_since_damage = -1;
    prop->currently_damaging_me = 0;
    prop->ticks_since_definitely_located = -1;
    prop->definitely_located = 0;
    prop->definite_knowledge_source_actor = -1;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop->last_perceived_time = -1;
    prop->last_visible_time = -1;
    prop->orphan_corpse_cheated = 0;
    prop->actor_index = -1;
    prop->___u3.orphan_prop_index = -1;
    prop->required_ticks = 0;
    prop->last_unreachable_time = -1;

    if ( unit_index != -1 )
    {
        unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        int16_t unit_team = unit_object->object.owner_team_index;
        unit_definition *unit_def = TAG_GET(unit_definition, unit_object->definition_index);

        prop->team_index = unit_team;
        prop->enemy = game_team_is_enemy(actor->meta.team_index, unit_team);
        prop->ally = game_team_is_ally(actor->meta.team_index, prop->team_index);
        prop->ally_status_changed = game_team_ally_status_changed(actor->meta.team_index, prop->team_index);

        int dead_now = (unit_object->object.damage_flags >> _object_dead_bit) & 1;
        prop->dead = (unit_object->object.damage_flags & (1u << _object_dead_bit)) != 0;
        prop->suicide_radius = unit_def->unit.ai_danger_radius;

        bool really_dead = 0;
        if ( dead_now )
            really_dead = unit_object->unit.feign_death_timer == 0;
        prop->really_dead = really_dead;
        prop->dead_ticks = dead_now ? 0x3E8 : 0;
        prop->player = unit_object->object.owner_player_index != -1;

        if ( unit_object->unit.swarm_actor_index == -1 )
        {
            prop->actor_index = unit_object->unit.actor_index;
        }
        else
        {
            prop->swarm = 1;
            prop->actor_index = unit_object->unit.swarm_actor_index;
            prop->swarm_unit_selected_time = game_time_get();
        }

        if ( prop->player )
        {
            prop->type = 6;
            prop->next_prop_index = actor->meta.first_prop_index;
            actor->meta.first_prop_index = prop_index;
            return;
        }

        if ( prop->actor_index != -1 )
        {
            prop->type =
                DATUM_GET(actor_data, actor_datum, prop->actor_index)->meta.type;
            prop->next_prop_index = actor->meta.first_prop_index;
            actor->meta.first_prop_index = prop_index;
            return;
        }

        prop->type = -1;
    }

    prop->next_prop_index = actor->meta.first_prop_index;
    actor->meta.first_prop_index = prop_index;
}
