#pragma once
/* hud_waypoint_definition — HUD navigation-waypoint block within hud_globals_definition.waypoint
 * (DB types_members-confirmed, 156 bytes). `arrows` is the tag_block of per-waypoint entries
 * consumed by find_nav_point.c (each entry's name string starts at its own offset 0). */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct hud_waypoint_definition
{
    float         top_offset;      /* 0x00 */
    float         bottom_offset;   /* 0x04 */
    float         left_offset;     /* 0x08 */
    float         right_offset;    /* 0x0C */
    int           unused0[8];      /* 0x10 */
    tag_reference arrow_bitmap;    /* 0x30 */
    tag_block     arrows;          /* 0x40 */
    int           unused1[20];     /* 0x4C */
} hud_waypoint_definition; /* 156 bytes */
