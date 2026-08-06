/* sound_is_queueable @ 0x84184B48 (.data, 44 bytes)
 * DB applied_types: unsigned __int8 sound_is_queueable[44];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000: 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01
 *   +0x0010: 01 01 01 01 01 01 01 01 01 01 00 00 00 00 00 00
 *   +0x0020: 01 01 01 01 01 01 01 01 01 01 00 00
 * Blam .lib data-global storage definition (init batch 7).
 *
 * Element count: 44, indexed by game_engine_multiplayer_sound. Three independent oracles agree:
 * the DB applied type is `unsigned __int8[44]`; the DB enum
 * _7F3B943EACD5D9B4DF3D550635507235 (headers/game_engine_multiplayer_sound.h) ends at
 * NUMBER_OF_MULTIPLAYER_SOUNDS = 44; and the sole consumer,
 * game_engine_play_multiplayer_sound_to_specific_player @0x837B6BE8, subscripts this with a
 * game_engine_multiplayer_sound index. The next .data name is
 * message_multiplayer_sound_definition @0x84184B78, i.e. 48 bytes away — that distance is an
 * UPPER bound that includes the 4 bytes of 16-byte alignment padding at +0x2C..+0x2F, which
 * are zero and belong to no object.
 *
 * DEVIATION: this array was authored `unsigned char sound_is_queueable[48]` with 48
 * initializers, four more than the object has. The extent had been taken from the
 * distance-to-next-symbol (48) rather than from the applied type (44); the four extra
 * elements are the inter-symbol alignment pad. All 44 real values were already correct.
 * The trailing zeros made the error invisible at run time, but the declared extent was wrong
 * and `sizeof(sound_is_queueable)` read 48.
 *
 * Value provenance, one per multiplayer sound:
 *   [ 0..25] = 1  oddball_spawn .. scorpion              queued
 *   [26..31] = 0  countdown_timer, teleporter_activate, flag_failure, countdown_for_respawn,
 *                 hill_move, respawn                     played immediately
 *   [32..41] = 1  team_king .. hill_occupied             queued
 *   [42..43] = 0  countdown_timer_end, ting              played immediately
 */
#include "../headers/game_engine_multiplayer_sound.h"

unsigned char sound_is_queueable[NUMBER_OF_MULTIPLAYER_SOUNDS] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,0,0
};
