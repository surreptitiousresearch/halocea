#pragma once

typedef struct game_input_preferences {
    float         yaw_rate;                          /* 0x00 */
    float         pitch_rate;                        /* 0x04 */
    unsigned char game_control_to_xbox_buttons[21];  /* 0x08 */
    unsigned char _pad0[1]; /* db-verified padding */
    short         joystick_controls;                 /* 0x1E */
    unsigned char invert_look;                       /* 0x20 */
    unsigned char invert_look_aircraft_control;      /* 0x21 */
} game_input_preferences;                            /* sizeof = 0x24 (36 bytes) */
