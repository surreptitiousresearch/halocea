#pragma once
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* player_profile — per-player saved settings/profile (player_profile.c). Canonical
 * definition; player_ui_globals.h carries an opaque stand-in for its embedded slots
 * (separate TUs, never included together). reserved tail kept opaque.
 * DB: types_members player_profile — controller block is the nested DB type
 * types_members player_profile::_controller_settings (spelled _controller_settings here;
 * C has no ::). */

typedef struct _controller_settings
{
    unsigned char button_preset;                  /* 0x0 */
    unsigned char joystick_preset;                /* 0x1 */
    unsigned char look_sensitivity;               /* 0x2 */
    unsigned char invert_look;                    /* 0x3 */
    unsigned char vibration_disabled;             /* 0x4 */
    unsigned char flight_stick_aircraft_controls; /* 0x5 */
    unsigned char autocenter;                     /* 0x6 */
    unsigned char ingame_help_disabled;           /* 0x7 */
    unsigned char game_control_to_xbox_buttons[21];/* 0x8 */
} _controller_settings;                           /* 29 bytes */

typedef struct player_profile
{
    unsigned char  profile_version;                      /* 0x000 */
    unsigned char  _pad1;                                /* 0x001 */
    wchar_t        player_name[12];                      /* 0x002 */
    unsigned char  player_name_reserved[256];            /* 0x01A */
    __int16        primary_color_index;                  /* 0x11A */
    unsigned __int16 flags;                              /* 0x11C */
    char           single_player_map_flags[10];          /* 0x11E */
    __int16        last_single_player_map_played;        /* 0x128 */
    _controller_settings controller_settings;            /* 0x12A */
    unsigned char  reserved[1652];                       /* 0x147 opaque remainder (DB u8[1652]) */
    unsigned char  _pad7DB[1];                           /* 0x7DB — trailing alignment */
} player_profile;                                        /* 1980 bytes */
