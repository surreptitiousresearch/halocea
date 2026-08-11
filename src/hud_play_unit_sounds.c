/* hud_play_unit_sounds @0x836D6800 — drive the unit HUD's warning sounds (shield low/recharge/empty,
 * health low/recharge/damage) for one player. It builds a bitfield of currently-active sound conditions by
 * comparing the unit's live shield/body vitality against the values cached in the player's unit_hud_state,
 * then hands that bitfield to hud_play_sound, which starts/stops looping sounds and remembers what is
 * playing. No sounds are evaluated while the unit is dead, when the HUD is hidden, during cinematics, or
 * for condition groups disabled by the HUD globals' script_flags.
 *
 * The flag bits are the DB enum $4F3E882FB0BAA1307C2B496DA45C66B2 (unit_hud_sound_state.h):
 * shield recharging/damage/low/empty (bits 0-3), health low/empty, minor/major damage (bits 4-7). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/object_flags.h"
#include "headers/object_damage_flags.h"
#include "headers/object_datum.h"
#include "headers/unit_hud_globals.h"
#include "headers/unit_hud_interface_definition.h"
#include "headers/unit_hud_sound_state.h"
#include "headers/hud_panel_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_block.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

struct unit_definition;


#include "headers/tag_block.h"
#include "headers/unit_definition.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int unit_definition_get_active_hud_index(const unit_definition *unit_definition, uint8_t in_multiplayer);
extern int16_t local_player_count(void);
extern uint8_t cinematic_in_progress(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern uint8_t game_engine_has_shield(int player_index);
extern void hud_play_sound(int16_t local_player_index, int type_flags, tag_block *sounds, int *sound_handles, uint16_t *sound_flags);

void hud_play_unit_sounds(const player_datum *player, uint8_t show_hud)
{
    int unit_index = player->unit_index;
    unit_hud_state *hud_state = &unit_hud_globals->hud_states[player->local_player_index];
    if (unit_index == -1)
        unit_index = hud_state->last_unit_index;

    object_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if (!unit)
        return;

    const struct unit_definition *unit_definition =
        TAG_GET(const struct unit_definition, unit->definition_index);
    int16_t player_count = local_player_count();
    int active_hud_index = unit_definition_get_active_hud_index(unit_definition,
                               ((player_count >= 0) + ((unsigned int)player_count <= 1)) & 1);
    if (active_hud_index == -1)
        return;

    int sound_flags = 0;
    unit_hud_interface_definition *hud_definition = TAG_GET(unit_hud_interface_definition, active_hud_index);

    if ((unit->object.flags & (1u << _object_on_media_bit)) != 0 || unit->object.body_vitality <= 0.0f)
    {
        hud_state->last_unit_index = -1;
    }
    else if (show_hud && !cinematic_in_progress())
    {
        /* Shield warnings. */
        if (hud_state->last_shield_vitality != -1.0f
            && game_engine_has_shield(local_player_get_player_index(player->local_player_index)))
        {
            if ((unit_hud_globals->script_flags & (1u << _hud_panel_shield_dont_show_bit)) == 0)
            {
                float shield = unit->object.shield_vitality;
                int flags = (unit->object.damage_flags >> _object_shield_charging_bit) & 1;  /* -> _unit_hud_shield_recharging */
                if (hud_state->last_shield_vitality > shield)
                    flags |= (1u << _unit_hud_shield_damage);
                if (shield >= 0.25f || shield <= 0.0f)
                    flags &= ~(1u << _unit_hud_shield_low);
                else
                    flags |= (1u << _unit_hud_shield_low);
                if (shield == 0.0f)
                    sound_flags = flags | (1u << _unit_hud_shield_empty);
                else
                    sound_flags = flags & ~(1u << _unit_hud_shield_empty);
            }
        }

        /* Body/health warnings. */
        if ((unit_hud_globals->script_flags & (1u << _hud_panel_health_dont_show_bit)) == 0)
        {
            float body = unit->object.body_vitality;
            if (body >= 0.25f)
                sound_flags &= ~(1u << _unit_hud_health_low);
            else
                sound_flags |= (1u << _unit_hud_health_low);
            if ((unit->object.damage_flags & (1u << _object_dead_bit)) != 0)
                sound_flags |= (1u << _unit_hud_health_empty);
            else
                sound_flags &= ~(1u << _unit_hud_health_empty);
            if (hud_state->last_body_vitality <= body || (hud_state->last_body_vitality - unit->object.body_vitality) >= 0.1875f)
                sound_flags &= ~(1u << _unit_hud_minor_damage);
            else
                sound_flags |= (1u << _unit_hud_minor_damage);
            if ((hud_state->last_body_vitality - unit->object.body_vitality) < 0.1875f)
                sound_flags &= ~(1u << _unit_hud_major_damage);
            else
                sound_flags |= (1u << _unit_hud_major_damage);
        }
    }

    hud_play_sound(player->local_player_index, sound_flags, &hud_definition->warning_sounds, /* was tag_block* + 80 (byte 960) */
                   hud_state->last_sound_handles, &hud_state->sound_flags);
}
