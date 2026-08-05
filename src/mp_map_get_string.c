#include <stdint.h>

extern uint8_t single_preview_column_list_get_string(uint16_t *out, int index, void *list_items);
extern char * map_list_get_level_name(int index);
extern uint8_t cache_file_map_exists(const char *name);

int mp_map_get_string(uint16_t *out, int index, void *list_items)
{
    if (!single_preview_column_list_get_string(out, index, list_items))
        return 0;

    return cache_file_map_exists(map_list_get_level_name(index));
}
