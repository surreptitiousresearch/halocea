#pragma once
#include <stdint.h>
/* _persistant_game_data_info — the most-recently-played campaign level/save marker. DB layout. */

typedef struct _persistant_game_data_info
{
    char             map_name[256];  /* 0x000 */
    int16_t          difficulty;     /* 0x100 */
    char             map_index;      /* 0x102 */
    uint8_t  valid;          /* 0x103 */
    uint8_t  corrupted;      /* 0x104 */
} _persistant_game_data_info;

extern _persistant_game_data_info persistant_game_data_info;
