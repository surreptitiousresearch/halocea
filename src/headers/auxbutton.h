#pragma once

/* auxbutton — the auxiliary-keyboard-button index shared by input_abstraction_globals.auxbutton_ticks[3]
 * and keyboard_auxbutton_mapping[3], and the `button` parameter of
 * input_abstraction_auxbutton_down @0x83724DC8. The index<->member correspondence is confirmed by the
 * data image of keyboard_auxbutton_mapping @0x84177F60 = { 16, 0, 13 }, which decodes through the
 * _key_* family ($433719FE2A052FABF0822A32CD2F9634) as _key_backquote (console), _key_escape (pause),
 * _key_print_screen (screenshot) — exactly this enum's order. Corroborated at the call sites:
 * console_update @0x83722B50 passes 0 to open/close the console, and main_game_render passes 2 inside
 * its EnabledScreenShot block. Source: types_enum_values $F6C2DE8FE0005540193AC452853EB0D6 (DB-verified). */
enum auxbutton {
	_auxbutton_console = 0,
	_auxbutton_pause = 1,
	_auxbutton_screenshot = 2,
	NUMBER_OF_AUXCONTROL_BUTTONS = 3,
};

// auxbutton: DB-verified via types_enum_values $F6C2DE8FE0005540193AC452853EB0D6
