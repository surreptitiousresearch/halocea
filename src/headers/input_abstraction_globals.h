#pragma once
/* _input_abstraction_globals @ input_abstraction_globals — input device/config state
 * (input_abstraction.c). Leading control/state arrays modeled opaque; the fields
 * touched by input_abstraction_initialize are named at their true offsets. */

#include "game_input_state.h"
#include "game_input_preferences.h"
#include "input_abstraction_input_button.h"
#include "gamepad_state.h"

typedef struct _input_abstraction_globals
{
    game_input_preferences player_control_settings[4]; /* 0x000 */
    game_input_state input_state[4];                   /* 0x090 */
    unsigned int  device_enumeration_startup_timer;    /* 0x120 */
    unsigned char controller_available[4];             /* 0x124 */
    unsigned char all_idle;                            /* 0x128 */
    unsigned char initialized;                         /* 0x129 */
    unsigned char active_modes;                        /* 0x12A */
    unsigned char _pad12B;                             /* 0x12B */
    gamepad_state configuration_gamepad_snapshot[4];   /* 0x12C — DB gamepad_state[4] */
    input_abstraction_input_button configuration_button_pressed; /* 0x1CC */
    unsigned char auxbutton_ticks[3];                  /* 0x1D8 */
    unsigned char _pad1DB[1];                          /* 0x1DB — trailing alignment */
} _input_abstraction_globals;                          /* 476 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern _input_abstraction_globals input_abstraction_globals;

#ifdef __cplusplus
}
#endif

/* device_usage_info_s — separate global (DB types_members device_usage_info_s); only
 * last_used_index touched here. */
extern struct device_usage_info_s { int last_used_index; } device_usage_info;

/* keyboard_auxbutton_mapping @ 0x84177F60 — __int16[3] aux-button keycode map (input_abstraction.c). */
extern short keyboard_auxbutton_mapping[3];
