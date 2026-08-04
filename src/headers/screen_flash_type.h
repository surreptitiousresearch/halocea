#ifndef SCREEN_FLASH_TYPE_H
#define SCREEN_FLASH_TYPE_H

/* screen_flash_type - tag-side blend mode of a screen_flash_definition.
 * DB-verified: types_enum_values $5DDBCA0FB6A5039E772D34FF95AA026E carries these named values verbatim. */
enum screen_flash_type {
	_screen_flash_type_none = 0,
	_screen_flash_type_lighten = 1,
	_screen_flash_type_darken = 2,
	_screen_flash_type_max = 3,
	_screen_flash_type_min = 4,
	_screen_flash_type_invert = 5,
	_screen_flash_type_tint = 6,
	NUMBER_OF_SCREEN_FLASH_TYPES = 7,
};

#endif /* SCREEN_FLASH_TYPE_H */
