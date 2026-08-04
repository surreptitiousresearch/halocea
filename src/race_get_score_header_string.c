/* race_get_score_header_string @0x8382B090 — fills `buffer` with the score-header string for the current
 * race variant: "laps" (string 178) normally, or "score" (string 25) when race is being played in the
 * "any" (terminator-style) scoring mode. */

#include <stdint.h>
#include "headers/game_variant.h"

extern game_variant *game_engine_get_variant(void);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrcpy(wchar_t *dest, const wchar_t *src);
extern const wchar_t empty_wide_string;

uint16_t * race_get_score_header_string(uint16_t *buffer)
{
    __int16 string_index = game_engine_get_variant()->game_engine_variant.terminator.ignored != 2 ? 178 : 25;

    int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *string = (text_tag == -1) ? &empty_wide_string : unicode_string_list_get_string(text_tag, string_index);

    ustrcpy(buffer, string);
    return buffer;
}
