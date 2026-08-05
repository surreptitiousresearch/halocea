#pragma once
#include <stdint.h>
/* scenario_bsp_switch_trigger_volume — one element of scenario.bsp_switch_trigger_volumes
 * (8 bytes, DB layout via types_members). When a unit enters trigger_volume_index while the
 * current structure BSP is source_bsp_index, the game switches to destination_bsp_index. */

typedef struct scenario_bsp_switch_trigger_volume
{
    int16_t trigger_volume_index;   /* 0x00 */
    int16_t source_bsp_index;       /* 0x02 */
    int16_t destination_bsp_index;  /* 0x04 */
    int16_t safe_flag_index;        /* 0x06 */
} scenario_bsp_switch_trigger_volume; /* 8 bytes */
