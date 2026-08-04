#pragma once
/* biped_contact_point — one element of _biped_definition.contact_points (64 bytes).
 * Layout DB-verified against type 'biped_contact_point' (types_members, 2026-07-14). */

typedef struct biped_contact_point
{
    int  unused[8];        /* 0x00 */
    char marker_name[32];  /* 0x20 */
} biped_contact_point;     /* 64 bytes */
