#pragma once
/* hud_waypoint_arrow — one entry of hud_globals->waypoint.arrows (DB types_members-confirmed,
 * 104 bytes). sequence_indices is indexed by the waypoint's screen state (0 = on-screen,
 * 1 = off-screen/edge-clamped, 2 = on-screen type-2). flags bit 0 disables edge-arrow rotation. */

typedef struct hud_waypoint_arrow
{
    char          name[32];            /* 0x00 */
    int           unused0[2];          /* 0x20 */
    unsigned int  color;               /* 0x28 — ARGB pixel32 */
    float         opacity;             /* 0x2C */
    float         fade;                /* 0x30 */
    __int16       sequence_indices[3]; /* 0x34 */
    __int16       pad;                 /* 0x3A */
    int           unused1[4];          /* 0x3C */
    unsigned int  flags;               /* 0x4C */
    int           unused2[6];          /* 0x50 */
} hud_waypoint_arrow; /* 104 bytes */
