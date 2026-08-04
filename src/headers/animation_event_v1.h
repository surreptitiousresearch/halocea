#pragma once

typedef struct animation_event_v1
{
    __int16           type;       /* 0x00 */
    unsigned __int16  time_delta; /* 0x02 */
} animation_event_v1;             /* 4 bytes */
