#pragma once
/* hud_globals_definition @ hud_globals — HUD tag globals (hud.c).
 * Full top-level layout is DB types_members-confirmed (see hud_render_nav_points.c /
 * find_nav_point.c consumers). */

#include "hud_messaging_parameters_definition.h"
#include "hud_waypoint_definition.h"
#include "hud_multiplayer_parameters_definition.h"
#include "hud_defaults_definition.h"
#include "hud_damage_indicators_definition.h"
#include "hud_timer_definition.h"
#include "tag_reference.h"

typedef struct hud_globals_definition
{
    hud_messaging_parameters_definition   messaging;         /* 0x000 (0) */
    hud_waypoint_definition               waypoint;          /* 0x120 (288) */
    hud_multiplayer_parameters_definition  multiplayer;       /* 0x1BC (444) */
    hud_defaults_definition               defaults;          /* 0x2C0 (704) */
    hud_damage_indicators_definition      damage_indicators; /* 0x310 (784) */
    hud_timer_definition                  timer_definition;  /* 0x360 (864) */
    tag_reference                         carnage_report_bitmap; /* 0x3C8 (968) */
    __int16       loading_begin_index;             /* 0x3D8 (984) */
    __int16       loading_end_index;               /* 0x3DA (986) */
    __int16       checkpoint_begin_index;          /* 0x3DC (988) */
    __int16       checkpoint_end_index;            /* 0x3DE (990) */
    tag_reference checkpoint_sound;                /* 0x3E0 (992) */
    int           unused2[24];                     /* 0x3F0 (1008) */
} hud_globals_definition;                          /* 1104 bytes */
