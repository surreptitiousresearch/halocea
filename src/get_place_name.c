/* get_place_name @0x83748F00 — resolve a game_engine_place to its display string in the
 * "ui\multiplayer_game_text" 'ustr' tag. Head-to-head (two participants) has dedicated strings:
 * tied-with-the-other (67), or place 0/1 rendered as "winning"/"losing" (65/66). Otherwise: all
 * participants tied selects a fixed string (64), a tie at this score uses the tied variant of the
 * place string (+32), else the plain place string. Final index is offset +102 into the string list.
 *
 * DEVIATION: the decompiler treats the by-value `game_engine_place` argument as a raw pointer (BYTE1(place),
 * (_WORD)place); DB confirms it's a 4-byte-by-value struct {flags, place} packed into one register — BYTE1
 * (memory-order byte 1) is flags' low byte, and the word-truncation casts are the place field. */

#include <stdint.h>
#include <stddef.h>   /* wchar_t */
#include "headers/game_engine_place.h"
#include "headers/game_engine_place_flags.h"

extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern unsigned __int16 empty_wide_string;

uint16_t * get_place_name(game_engine_place place)
{
    __int16 string_index;

    if ( (place.flags & (1 << _place_two_groups)) != 0 && (place.flags & (1 << _place_tied)) != 0 )
    {
        string_index = 67;
    }
    else if ( (place.flags & (1 << _place_two_groups)) != 0 && place.place == 0 )
    {
        string_index = 65;
    }
    else if ( (place.flags & (1 << _place_two_groups)) != 0 && place.place == 1 )
    {
        string_index = 66;
    }
    else if ( (place.flags & (1 << _place_all_tied)) != 0 )
    {
        string_index = 64;
    }
    else if ( (place.flags & (1 << _place_tied)) != 0 )
    {
        string_index = place.place + 32;   /* tied variant of the place string */
    }
    else
    {
        string_index = place.place;
    }

    int tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    if ( tag_index == -1 )
        return &empty_wide_string;
    return unicode_string_list_get_string(tag_index, string_index + 102);
}
