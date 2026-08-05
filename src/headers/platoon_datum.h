#pragma once
#include <stdint.h>
/* platoon_datum — runtime per-platoon state (16 bytes, full DB layout). */

typedef struct platoon_datum
{
    uint8_t defending;                    /* 0x00 */
    uint8_t maneuvering;                  /* 0x01 */
    uint8_t maneuver_disable;             /* 0x02 */
    uint8_t _pad03;                       /* 0x03 */
    int16_t         original_count;               /* 0x04 */
    int16_t         current_count;                /* 0x06 */
    int16_t         current_swarm_count;          /* 0x08 */
    int16_t         _pad0A;                        /* 0x0A */
    float           current_strength_fraction;    /* 0x0C */
} platoon_datum;                                  /* 16 bytes */
