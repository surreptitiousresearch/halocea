#pragma once
/* buildin_multiplayer_map_s — build-time table source for map_list_initialize_known_list */

typedef struct buildin_multiplayer_map_s
{
    int           map_index;          /* 0x0 */
    const char   *map_name;           /* 0x4 */
    unsigned char original_xbox_map;  /* 0x8 */
} buildin_multiplayer_map_s; /* 12 bytes */
