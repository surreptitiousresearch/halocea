/* input_abstraction_get_game_control_byname @0x837255E8 — DEVIATION: the decompiler expressed the
 * table-end test against an unrelated adjacent float-constant symbol (&_real_3fe38c3550000000). Per the
 * names table, control_to_name_table sits at 0x82115780 and that constant at 0x821158D0 — a gap of 0x150
 * (336) bytes, i.e. exactly 21 16-byte entries — so the scan is reproduced as an index-bounded search
 * (same fix class as create_mutex.c/get_mutex_from_pool.c). */

#include "headers/control_to_name_entry.h"
#include "headers/control_button.h"

extern int stricmp(const char *string1, const char *string2);

int input_abstraction_get_game_control_byname(const char *control_specifier)
{
    for ( int i = 0; i < CONTROL_TO_NAME_TABLE_COUNT; i++ )
    {
        if ( !stricmp(control_specifier, control_to_name_table[i].name) )
            return i;
    }
    return _button_none;
}
