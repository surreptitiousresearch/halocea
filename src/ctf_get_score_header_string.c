/* ctf_get_score_header_string @0x83806DE8 */
#include <stdint.h>
#include <stddef.h>
#include "headers/multiplayer_game_text_string.h"

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrcpy(wchar_t *dest, const wchar_t *src);

uint16_t * ctf_get_score_header_string(uint16_t *buffer)
{
    /* 'ustr' unicode_string_list tag; index 154 = _string_score (multiplayer_game_text_string) */
    int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *string = (text_tag == -1) ? empty_wide_string
            : unicode_string_list_get_string(text_tag, _string_score);

    ustrcpy(buffer, string);
    return buffer;
}
