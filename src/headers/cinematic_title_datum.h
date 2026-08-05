#pragma once
#include <stdint.h>
/* cinematic_title_datum @ cinematic.c — one active cinematic subtitle slot. */

typedef struct cinematic_title_datum
{
    int16_t title_index; /* 0x00 */
    int16_t title_timer; /* 0x02 */
} cinematic_title_datum; /* 4 bytes */
