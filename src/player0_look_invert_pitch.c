/* player0_look_invert_pitch @0x83699A20 — set local player 0's invert-look setting and, if the player has a
 * bound profile, notify every active player's HUD with a "setting changed" string, persist the profile to
 * disk, and re-apply the control bindings. */

#include <wchar.h>
#include <stdint.h>
#include "headers/player_ui_profile_view.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"

#include "headers/player_profile.h"
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void hud_print_message(int16_t local_player_index, const wchar_t *text);
extern void player_profile_save(int player_profile_index, player_profile *profile);
extern void set_local_player_controls_from_player_profile(int16_t local_player_index);
extern const wchar_t empty_wide_string;   /* empty wide string fallback */

void player0_look_invert_pitch(uint8_t invert)
{
    player_ui_globals.player_data[0].profile.controller_settings.invert_look = invert;

    if ( player_ui_globals.player_data[0].profile_index != -1 )
    {
        int strings_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\shell\\strings\\temp_strings");
        const wchar_t *message = strings_tag == -1
                               ? &empty_wide_string
                               : unicode_string_list_get_string(strings_tag, 1);

        data_iterator iterator;
        data_iterator_new(&iterator, player_data);
        for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
        {
            /* sentinel: original read was unsigned __int16; keep the cast so -1 compares as 0xFFFF */
            int hud_player_index = (unsigned __int16)player->local_player_index;
            if ( hud_player_index != 0xFFFF )
                hud_print_message(hud_player_index, message);
        }

        player_profile_save(player_ui_globals.player_data[0].profile_index, &player_ui_globals.player_data[0].profile);
    }

    set_local_player_controls_from_player_profile(0);
}
