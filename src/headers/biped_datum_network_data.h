#pragma once
#include <stdint.h>
/* biped_datum_network_data — a biped's networked baseline snapshot (16 bytes, DB types_members-confirmed). */

typedef struct biped_datum_network_data
{
    char             grenade_counts[2];                    /* 0x0 */
    unsigned char _pad0[2]; /* db-verified padding */
    float            body_vitality;                        /* 0x4 */
    float            shield_vitality;                       /* 0x8 */
    uint8_t  shield_stun_ticks_greater_than_zero;    /* 0xC */
} biped_datum_network_data; /* 16 bytes */
