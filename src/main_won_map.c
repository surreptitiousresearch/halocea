/* main_won_map @0x8368B0A8 — end-of-level "won" handling for solo play: stop any cinematic, then (unless in
 * the anitec render mode) flag the map as won, and on the second mission (solo level id 2) scan each local
 * player's unit inventory for the sniper rifle with >= 4 total rounds to award achievement ACH23. Awards
 * ACH44 on heroic+, then fires the "game_won_<map>" and "game_won" (level_id=...) script events.
 *
 * Deviations: player/object/tag lookups use the engine's raw datum-array address arithmetic (datum stride
 * and index masking) reproduced verbatim from the decompiler; the inventory weapon-slot base (dword 190),
 * the unit's loaded/unloaded round counts (object +694 / +696), and the player's unit-object index
 * (player datum +52) are raw offsets. The weapon-name compare is an inline strcmp against the tag path. */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/weapon_datum.h"
#include "headers/unit_datum.h"
#include "headers/single_player_progress_flags.h"
#include "headers/game_difficulty.h"
#include <string.h>
#include "headers/blam_data_globals.h"


extern int16_t main_get_solo_level_from_name(const char *name);
extern void hcex_stop_cine(void);
extern int hcex_is_anitec_render_mode(void);
extern int16_t local_player_get_next(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern void hcex_fire_plr_event(const char *event_name, int player_index);
extern int16_t game_difficulty_level_get(void);
extern void hcex_check_ach44(void);
extern void hcex_fire_event(const char *event_name);
extern void hcex_fire_event_s(const char *event_name, const char *param_name, const char *param_value);
extern char *tag_get_name(int tag_index);
/* strlwr provided by CRT via <string.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);

void main_won_map(void)
{
    int16_t solo_level = main_get_solo_level_from_name(main_globals.soloplayer_map_name);
    hcex_stop_cine();
    if ( hcex_is_anitec_render_mode() )
        return;

    hcex_load_checkpoint = 0;
    main_globals.save_map = 0;
    main_globals.won_map = 1;

    if ( solo_level == _single_player_map_a50 )  /* the sniper-rifle mission */
    {
        uint32_t *object_data = 0;
        for ( int16_t local_player = local_player_get_next(-1);
              local_player != -1;
              local_player = local_player_get_next(local_player) )
        {
            int player_index = local_player_get_player_index(local_player);
            if ( player_index == -1 )
                continue;

            int unit_object_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
            if ( unit_object_index == -1 )
                continue;

            unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_object_index)->datum;

            weapon_datum *weapon_object = 0;   /* set within the slot loop */
            int found_sniper = 0;
            for ( int slot = 0; slot < 4; slot = (int16_t)(slot + 1) )
            {
                /* DEVIATION: raw dword 190+slot is unit.weapon_object_indices[slot] (unit@500 + 260 + 4*slot) */
                int weapon_index = unit_object->unit.weapon_object_indices[slot];
                if ( weapon_index == -1 )
                    continue;

                weapon_object = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
                int weapon_tag_index = *(uint32_t *)weapon_object;
                /* DEVIATION FIX: restored missing deref — DB reads (*(_DWORD **)slot)[11] = definition
                 * dword 11 = _object_definition.model.name (weapon's model tag path) */
                char *weapon_name = TAG_GET(_object_definition, weapon_tag_index)->model.name;
                if ( weapon_name && !strcmp(weapon_name, "weapons\\sniper rifle\\sniper rifle") )
                {
                    found_sniper = 1;
                    break;
                }
            }

            if ( found_sniper
                && weapon_object->weapon.magazines[0].rounds_loaded
                    + weapon_object->weapon.magazines[0].rounds_total >= 4 )
            {
                hcex_fire_plr_event("ACH23", local_player);
            }
        }
    }

    if ( game_difficulty_level_get() >= game_difficulty_level_hard )
        hcex_check_ach44();

    char event_name[336];
    char *map_leaf = strrchr(main_globals.soloplayer_map_name, '\\');
    _snprintf_0(event_name, 0xFFu, "game_won_%s", map_leaf + 1);
    event_name[255] = 0;
    strlwr(event_name);
    hcex_fire_event(event_name);

    char *scenario_name = tag_get_name(global_scenario_index);
    const char *level_id = strlwr(strrchr(scenario_name, '\\') + 1);
    hcex_fire_event_s("game_won", "level_id", level_id);
}
