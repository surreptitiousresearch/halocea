#pragma once
/* _persistant_game_data_info — the most-recently-played campaign level/save marker. DB layout. */

typedef struct _persistant_game_data_info
{
    char             map_name[256];  /* 0x000 */
    __int16          difficulty;     /* 0x100 */
    char             map_index;      /* 0x102 */
    unsigned __int8  valid;          /* 0x103 */
    unsigned __int8  corrupted;      /* 0x104 */
} _persistant_game_data_info;

extern _persistant_game_data_info persistant_game_data_info;
