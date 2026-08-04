#ifndef HUD_METER_FLAGS_H
#define HUD_METER_FLAGS_H

/* hud_meter_flags — hud_meter flags.
 * DB-verified: types_enum_values _150FFC51872CC8B1DB795F18AC9585FF carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum hud_meter_flags {
	_hud_meter_switch_color_on_state_change = 0,
	_hud_meter_interpolates_between_min_max_bit = 1,
	_hud_meter_interpolate_in_hsv_space_bit = 2,
	_hud_meter_interpolate_along_farthest_hue_path_bit = 3,
	_hud_meter_invert_interpolation_value_bit = 4,
	NUMBER_OF_HUD_METER_FLAGS = 5,
};

#endif /* HUD_METER_FLAGS_H */

// hud_meter_flags: DB-verified via types_enum_values _150FFC51872CC8B1DB795F18AC9585FF
