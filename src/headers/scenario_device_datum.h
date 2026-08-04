#pragma once
/* scenario_device_datum — device-object scenario placement sub-block (DB types_members-confirmed, 8 bytes). */

typedef struct scenario_device_datum
{
    __int16       power_group_index;    /* 0x00 */
    __int16       position_group_index; /* 0x02 */
    unsigned int  flags;                /* 0x04 */
} scenario_device_datum; /* 8 bytes */
