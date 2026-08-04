#ifndef WAYPOINT_TYPE_H
#define WAYPOINT_TYPE_H

/* waypoint_type — HUD nav-point / waypoint screen-visibility classification used to select the
 * waypoint arrow render mode (hud_get_nav_point_render_type result; custom_render_nav_point selector).
 * DB-verified: types_enum_values _2920330745BE69988217B54BB2252B26 carries these values verbatim. */
enum waypoint_type {
	_waypoint_on_screen = 0,
	_waypoint_off_screen = 1,
	_waypoint_occluded = 2,
	NUMBER_OF_WAYPOINT_TYPES = 3,
};

#endif /* WAYPOINT_TYPE_H */
