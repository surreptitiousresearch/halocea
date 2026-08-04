#pragma once
/* platoon_datum — runtime per-platoon state (16 bytes, full DB layout). */

typedef struct platoon_datum
{
    unsigned __int8 defending;                    /* 0x00 */
    unsigned __int8 maneuvering;                  /* 0x01 */
    unsigned __int8 maneuver_disable;             /* 0x02 */
    unsigned __int8 _pad03;                       /* 0x03 */
    __int16         original_count;               /* 0x04 */
    __int16         current_count;                /* 0x06 */
    __int16         current_swarm_count;          /* 0x08 */
    __int16         _pad0A;                        /* 0x0A */
    float           current_strength_fraction;    /* 0x0C */
} platoon_datum;                                  /* 16 bytes */
