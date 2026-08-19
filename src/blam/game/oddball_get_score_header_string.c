/* oddball_get_score_header_string @0x83816B18 — formats the oddball score-column header string: string 158
 * for non-terminator oddball variants, 154 (the same index ctf_get_score_header_string.c uses) for
 * terminator variants. Mirrors ctf_get_score_header_string.c's fallback-to-empty_wide_string pattern. */

#include <stdint.h>
#include "headers/game_variant.h"

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */
extern game_variant *game_engine_get_variant(void);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrcpy(wchar_t *dest, const wchar_t *src);

uint16_t * oddball_get_score_header_string(uint16_t *buffer)
{
    int16_t string_index =
        game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type != _oddball_terminator ? 158 : 154;

    int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
    const wchar_t *string = (text_tag == -1) ? empty_wide_string : unicode_string_list_get_string(text_tag, string_index);

    ustrcpy(buffer, string);
    return buffer;
}
