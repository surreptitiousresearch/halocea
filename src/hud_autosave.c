/* hud_autosave @ 0x83708FA8 — show the checkpoint (begin) or checkpoint-done (end)
 * caption to each local player, and on begin play the checkpoint sound. */
#include <wchar.h>
#include <stdint.h>
#include "headers/hud_globals.h"

extern void scripted_hud_messages_clear(void);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);
extern int local_player_get_player_index(int16_t local_player_index);
#include "headers/global_tag_instances.h"  /* global_tag_instances; base_address@0x14 (tag-instance table) */
#include "headers/blam_data_globals.h"
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void hud_print_message(int16_t local_player_index, const wchar_t *text);

void hud_autosave(uint8_t begin)
{
    __int16 caption_index = begin ? hud_globals->checkpoint_begin_index
                                  : hud_globals->checkpoint_end_index;
    __int16 i;

    scripted_hud_messages_clear();

    if ( begin )
    {
        int sound_index = hud_globals->checkpoint_sound.index;
        if ( sound_index != -1 )
            unspatialized_impulse_sound_new(sound_index, 1.0f);
    }

    if ( caption_index == -1 )
        return;

    for ( i = 0; i < 2; ++i )
    {
        const wchar_t *string;
        int messages_index;

        if ( local_player_get_player_index(i) == -1 )
            continue;

        string = default_string;
        messages_index = hud_globals->messaging.hud_item_messages.index;
        if ( messages_index != -1 )
        {
            /* resolve the unicode_string_list tag; first dword is the string count */
            int *string_list = TAG_GET(int, messages_index);
            if ( string_list && caption_index >= 0 && caption_index < *string_list )
                string = unicode_string_list_get_string(
                    hud_globals->messaging.hud_item_messages.index, caption_index);
        }
        hud_print_message(i, string);
    }
}
