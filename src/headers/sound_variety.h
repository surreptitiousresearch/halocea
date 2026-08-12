#pragma once

/* sound_variety — the sound-variety quality setting stored in sound_manager_globals.sound_variety
 * (short @0xB8, see src/headers/sound_manager_globals_t.h). sound_get_variety /
 * sound_set_variety are its only accessors, and sound_initialize (@0x83716838) defaults it to the
 * low setting alongside the other named sound-manager defaults. The DB carries no NUMBER_OF_*
 * terminator for this family. Source: types_enum_values $58C1A69FCC7FFE3E48BC5D8B5FE5E6DD
 * (DB-verified). */
enum sound_variety {
	_sound_variety_low = 0,
	_sound_variety_medium = 1,
	_sound_variety_high = 2,
};

// sound_variety: DB-verified via types_enum_values $58C1A69FCC7FFE3E48BC5D8B5FE5E6DD
