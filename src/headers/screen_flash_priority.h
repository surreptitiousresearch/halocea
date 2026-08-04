#ifndef SCREEN_FLASH_PRIORITY_H
#define SCREEN_FLASH_PRIORITY_H

/* screen_flash_priority - priority slot of a screen_flash_definition.
 * DB-verified: types_enum_values $3B48337005EF9B3E043A7292131C63DB carries these named values verbatim. */
enum screen_flash_priority {
	_screen_flash_low_priority = 0,
	_screen_flash_medium_priority = 1,
	_screen_flash_high_priority = 2,
	NUMBER_OF_SCREEN_FLASH_PRIORITIES = 3,
};

#endif /* SCREEN_FLASH_PRIORITY_H */
