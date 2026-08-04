#ifndef HUD_WAYPOINT_ARROW_FLAGS_H
#define HUD_WAYPOINT_ARROW_FLAGS_H

/* hud_waypoint_arrow flags — bit indices for hud_waypoint_arrow.flags.
 * DB-verified: types_enum_values _78C641C61483D70BC47E67604E14FD0F carries these values verbatim.
 * Values are BIT INDICES — use as (1u << _bit). */
enum hud_waypoint_arrow_flags {
	_hud_waypoint_dont_rotate_offscreen = 0,
	NUMBER_OF_WAYPOINT_FLAGS = 1,
};

#endif /* HUD_WAYPOINT_ARROW_FLAGS_H */
