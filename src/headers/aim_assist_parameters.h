#pragma once
/* aim_assist_parameters — autoaim / magnetism / deviation angles and distances for a weapon at a given zoom
 * level, scaled by zoom magnification (and the HCEX aim-assist coefficient for players). Layout from the
 * database. */

typedef struct aim_assist_parameters
{
    float autoaim_angle;       /* 0x00 */
    float autoaim_distance;    /* 0x04 */
    float magnetism_angle;     /* 0x08 */
    float magnetism_distance;  /* 0x0C */
    float deviation_angle;     /* 0x10 */
    float unused;              /* 0x14 */
} aim_assist_parameters;
