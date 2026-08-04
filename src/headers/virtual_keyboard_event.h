#ifndef VIRTUAL_KEYBOARD_EVENT_H
#define VIRTUAL_KEYBOARD_EVENT_H

/* virtual_keyboard_event — the virtual keyboard's navigation/action event code, plus the
 * keyboard grid dimensions (same DB constants bag). */
enum virtual_keyboard_event {
	_event_tab_left = 0,
	_event_tab_right = 1,
	_event_tab_up = 2,
	_event_tab_down = 3,
	_event_key_select = 4,
	_event_cancel = 5,
	NUMBER_OF_VIRTUAL_KEYBOARD_EVENTS = 6,
};

enum virtual_keyboard_grid {
	NUMBER_OF_VIRTUAL_KEYBOARD_ROWS = 5,
	NUMBER_OF_VIRTUAL_KEYBOARD_COLUMNS = 11,
};

#endif /* VIRTUAL_KEYBOARD_EVENT_H */

// virtual_keyboard_event: DB-verified via types_enum_values $18F5DF1C5BF29BE371284E52B7A19B86
