#pragma once
#include <stdint.h>

typedef struct ai_allegiance_information_data
{
    int16_t         team1_index;   /* 0x00 */
    int16_t         team2_index;   /* 0x02 */
    uint8_t broken;        /* 0x04 */
} ai_allegiance_information_data;  /* 6 bytes */
