/* network_game_get_random_player_name @0x83818650 — pick a random name from the "ui\random_player_names"
 * unicode_string_list tag (used to seed a joining player's default name). Falls back to the empty-string
 * constant if the tag isn't loaded or its definition data is missing. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include <stddef.h>   /* wchar_t */

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */

extern int tag_loaded(uint32_t group_tag, const char *name);
extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);

uint16_t * network_game_get_random_player_name(void)
{
    int tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\random_player_names");
    if ( tag_index == -1 )
        return (uint16_t *)empty_wide_string;

    int16_t *string_list = TAG_GET(int16_t, tag_index);
    if ( !string_list )
        return (uint16_t *)empty_wide_string;

    int16_t max_index = *string_list - 1;
    unsigned int *seed = get_global_local_random_seed_address();
    int16_t chosen_index = seed_random_range(seed, 0, max_index);

    return unicode_string_list_get_string(tag_index, chosen_index);
}
