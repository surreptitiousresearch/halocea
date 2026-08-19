/* map_list_get_friendly_level_name @0x83767690 — produce the wide friendly name for a map: for a known
 * built-in multiplayer map (index <= 0x12) look up its localized picture-name string; otherwise widen the
 * map's leaf filename.
 *
 * Deviation: 0x75737472 is the 'ustr' (unicode_string_list) group tag. */

#include <stdint.h>
#include "headers/multiplayer_map_s.h"
#include "headers/multiplayer_map_constants.h"
#include <wchar.h>
#include "headers/blam_data_globals.h"

extern int tag_loaded(uint32_t group_tag, const char *name);
extern int map_list_find_map(const char *map_name);
/* strrchr provided by CRT via <string.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

/* map_name const-qualified 2026-07-30: read-only (find/strrchr/widen), callers pass const strings (C4090) */
void map_list_get_friendly_level_name(const char *map_name, uint16_t *name_buf, int name_buf_len)
{
    int string_list = tag_loaded(0x75737472u, "ui\\shell\\main_menu\\mp_map_list");
    unsigned int map = map_list_find_map(map_name);
    if ( map >= NUMBER_OF_BUILTIN_MULTIPLAYER_MAPS )   /* deviation: literal > 0x12 rewritten as >= 19 */
    {
        const char *leaf = strrchr(map_name, '\\');
        ascii_to_wide(leaf ? leaf + 1 : map_name, name_buf, 2 * name_buf_len);
    }
    else
    {
        const wchar_t *string = unicode_string_list_get_string(string_list, multiplayer_maps[map].pic_name_index);
        ustrncpy(name_buf, string, name_buf_len - 1);
        name_buf[name_buf_len - 1] = 0;
    }
}
