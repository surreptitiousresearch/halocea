#pragma once
/* flag_attachment_point — one element of flag_definition.attachment_points (52 bytes, DB layout
 * via types_members flag_attachment_point). Each scripted attachment point spans
 * height_to_next_attachment rows of the cloth grid and is anchored to a named object marker. */

typedef struct flag_attachment_point
{
    __int16          height_to_next_attachment; /* 0x00 */
    unsigned __int16 pad;                        /* 0x02 */
    int              unused[4];                  /* 0x04 */
    char             marker_name[32];            /* 0x14 (20) */
} flag_attachment_point;                          /* 52 bytes */
