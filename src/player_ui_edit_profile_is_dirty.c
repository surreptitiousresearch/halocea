/* player_ui_edit_profile_is_dirty @0x836990C8 — true if the in-progress edit slot differs from the
 * original snapshot. Both edit_profile_data.edit_profile and .original_profile are 1980-byte slots
 * viewed either as a game_variant (saved_game_file_get_type()==1, editing a game variant) or a
 * player_profile (any other type, editing a controller/input profile) — see player_ui_globals.h. The
 * comparison is a raw byte-by-byte scan of the whole slot with the type-appropriate `flags` field
 * temporarily zeroed on both sides first, so flag differences don't count toward dirtiness.
 *
 * DEVIATION: the decompiler renders this as a struct-pointer walk incrementing by 1 and re-reading a
 * "profile_version" field each step; profile_version sits at byte offset 0 of player_profile, so that is
 * just an obfuscated byte-by-byte memory comparison — reproduced here as one. */

#include <stdint.h>
#include "headers/player_ui_globals.h"
#include "headers/game_globals.h"

extern uint16_t saved_game_file_get_type(int index);

uint8_t player_ui_edit_profile_is_dirty(void)
{
    uint8_t dirty = 0;

    if ( player_ui_globals.edit_profile_data.profile_index != -1 )
    {
        uint16_t saved_game_type = saved_game_file_get_type(player_ui_globals.edit_profile_data.profile_index);
        if ( saved_game_type )
        {
            /* edit_profile/original_profile are union values, not pointers: take their address */
            unsigned char *edit_slot = (unsigned char *)&player_ui_globals.edit_profile_data.edit_profile;
            unsigned char *original_slot = (unsigned char *)&player_ui_globals.edit_profile_data.original_profile;
            unsigned int flags_offset = (saved_game_type == 1) ? 0x94 : 0x11C; /* game_variant.flags vs player_profile.flags */

            uint16_t original_flags = *(uint16_t *)(original_slot + flags_offset);
            uint16_t edit_flags = *(uint16_t *)(edit_slot + flags_offset);
            *(uint16_t *)(original_slot + flags_offset) = 0;
            *(uint16_t *)(edit_slot + flags_offset) = 0;

            unsigned char *p_original = original_slot;
            unsigned char *p_edit = edit_slot;
            unsigned char *end = original_slot + sizeof(player_ui_globals.edit_profile_data.original_profile); /* 1980-byte profile union */
            int byte_diff = 0;
            while ( p_original != end )
            {
                byte_diff = *p_original - *p_edit;
                if ( *p_original != *p_edit )
                    break;
                p_original++;
                p_edit++;
            }
            if ( byte_diff )
                dirty = 1;

            *(uint16_t *)(original_slot + flags_offset) = original_flags;
            *(uint16_t *)(edit_slot + flags_offset) = edit_flags;
        }
    }

    return dirty;
}
