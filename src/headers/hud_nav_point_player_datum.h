#pragma once
/* hud_nav_point_player_datum — per-player HUD nav-point slot table (DB layout, 48 bytes):
 * four hud_nav_point_datum entries. */

#include "hud_nav_point_datum.h"

typedef struct hud_nav_point_player_datum
{
    hud_nav_point_datum nav_points[4]; /* 0x0 */
} hud_nav_point_player_datum;          /* 48 bytes */
