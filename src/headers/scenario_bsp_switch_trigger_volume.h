#pragma once
/* scenario_bsp_switch_trigger_volume — one element of scenario.bsp_switch_trigger_volumes
 * (8 bytes, DB layout via types_members). When a unit enters trigger_volume_index while the
 * current structure BSP is source_bsp_index, the game switches to destination_bsp_index. */

typedef struct scenario_bsp_switch_trigger_volume
{
    __int16 trigger_volume_index;   /* 0x00 */
    __int16 source_bsp_index;       /* 0x02 */
    __int16 destination_bsp_index;  /* 0x04 */
    __int16 safe_flag_index;        /* 0x06 */
} scenario_bsp_switch_trigger_volume; /* 8 bytes */
