#pragma once
/* Bound to DB enum types_enum_values _ADFEA13EE5F8CFFC409A46AEC7B4FFCB (2026-07-20 BLOCKED-retry). */

/* director_globals.game_mode. DB-AUTHORITATIVE (was a GUESS before extraction). Canonical DB names
 * below; the consumer (director_choose_camera) keeps its guessed identifiers as same-value aliases.
 * Prior guesses corrected: 1 was "gameplay_alt" (DB _director_mode_netgame), 3 was "none"
 * (DB _director_mode_scripted), 4 was "scripted" (DB _director_mode_script_camera_record). Numeric
 * values are unchanged so consumer dispatch is unaffected. */
enum director_game_mode
{
    _director_mode_game                = 0,
    _director_game_mode_gameplay       = 0, /* legacy consumer alias */
    _director_mode_netgame             = 1,
    _director_game_mode_gameplay_alt   = 1, /* legacy consumer alias (guessed name) */
    _director_mode_editor              = 2,
    _director_game_mode_editor         = 2, /* legacy consumer alias */
    _director_mode_scripted            = 3,
    _director_game_mode_none           = 3, /* legacy consumer alias (guessed name) */
    _director_mode_script_camera_record = 4,
    _director_game_mode_scripted       = 4, /* legacy consumer alias (guessed name) */
    NUMBER_OF_DIRECTOR_GAME_MODES = 5,
};
