#ifndef ICON_FLAGS_H
#define ICON_FLAGS_H

/* icon_flags — icon flags.
 * DB-verified: types_enum_values _25E7FAC27FBFCB5F9F589BBE45394C23 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum icon_flags {
	_hud_icon_use_text_bit = 0,
	_hud_icon_use_color_bit = 1,
	_hud_icon_absolute_width_bit = 2,
	NUMBER_OF_ICON_FLAGS = 3,
};

#endif /* ICON_FLAGS_H */

// icon_flags: DB-verified via types_enum_values _25E7FAC27FBFCB5F9F589BBE45394C23
