#pragma once
/* cinematic_title_datum @ cinematic.c — one active cinematic subtitle slot. */

typedef struct cinematic_title_datum
{
    __int16 title_index; /* 0x00 */
    __int16 title_timer; /* 0x02 */
} cinematic_title_datum; /* 4 bytes */
