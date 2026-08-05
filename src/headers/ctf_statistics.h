#pragma once
#include <stdint.h>
/* ctf_statistics — CTF arm of the multiplayer_statistics union (6 bytes, DB types_members-confirmed). */

typedef struct ctf_statistics
{
    int16_t flag_grabs;   /* 0x00 */
    int16_t flag_returns; /* 0x02 */
    int16_t flag_scores;  /* 0x04 */
} ctf_statistics; /* 6 bytes */
