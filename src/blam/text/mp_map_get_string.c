/* mp_map_get_string @0x837813F8 — multiplayer map-list row text callback: fill the row string from the
 * generic single-preview-column list, then report whether that map's cache file actually exists.
 *
 * Deviation: attested uint8_t, not int — the function is only ever reached through the
 * `uint8_t (*)(uint16_t *, int, void *)` callback slot of single_preview_column_list_update (installed by
 * mp_map_single_preview_column_list_update @0x83781458, which grays the row on a false return), and both
 * return paths inherit an attested uint8_t (the early-out r3 is single_preview_column_list_get_string's
 * own zero; the tail is cache_file_map_exists). No widening occurs anywhere. */

#include <stdint.h>

extern uint8_t single_preview_column_list_get_string(uint16_t *out, int index, void *list_items);
extern char * map_list_get_level_name(int index);
extern uint8_t cache_file_map_exists(const char *name);

uint8_t mp_map_get_string(uint16_t *out, int index, void *list_items)
{
    if (!single_preview_column_list_get_string(out, index, list_items))
        return 0;

    return cache_file_map_exists(map_list_get_level_name(index));
}
