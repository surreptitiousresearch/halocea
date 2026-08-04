/* get_place_string @0x837464C0 — resolves a statistic_buffer row's "place" ranking (masked to the low 7 bits,
 * clamped to 31) to its localized ordinal string ("1st"/"2nd"/.../tied variants) from
 * "ui\multiplayer_game_text"'s unicode string list (base string index 36 + place); falls back to an empty
 * string when that tag isn't loaded. */

#include <stdint.h>
#include <stddef.h>   /* wchar_t */
#include "headers/statistic_buffer.h"
#include "headers/game_engine_place_constants.h"

extern const wchar_t empty_wide_string; /* .short 0 — empty wide string, "tag not loaded" fallback */

extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);

uint16_t * get_place_string(statistic_buffer *buffer)
{
    unsigned int place = buffer->place & 0x7F;
    if (place >= maximum_places)   /* deviation: literal 0x1F/31 = maximum_places - 1 */
        place = maximum_places - 1;

    int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    if (text_tag == -1)
        return (unsigned __int16 *)&empty_wide_string;

    return unicode_string_list_get_string(text_tag, place + 36);
}
