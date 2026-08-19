/* game_engine_play_multiplayer_sound_to_specific_player @0x837B6BB0 — plays a multiplayer sound for a
 * player, queueing it if the sound is marked queueable so that overlapping requests are spaced out. A
 * queued entry records the sound's playback duration (in ticks) plus a 5-tick guard; only when the queue
 * transitions from empty to one entry (or the sound is not queueable) is it played immediately. Sounds
 * only replicate while we are an active client/server (game_connection 2). */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/queued_mp_sound.h"
#include "headers/game_globals_definition.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/tag_reference.h"
#include "headers/sound_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/game_connection.h"

extern int game_engine_queued_sound_count;   /* number of queued sounds */

extern int16_t game_connection(void);
/* wrong-callee fix (trap h): binary bl targets the internal 3-arg _game_engine_play_multiplayer_sound,
 * not the public 2-arg thunk (which is li r3,-1 + tail b into it). */
extern void _game_engine_play_multiplayer_sound(int player_index, int index, uint8_t should_replicate);
extern int get_sound_length_in_ticks(int index);

void game_engine_play_multiplayer_sound_to_specific_player(int player_index, int index, uint8_t should_replicate)
{
    uint8_t replicate = game_connection() != _game_connection_network_server ? 0 : should_replicate;

    if (!sound_is_queueable[index])
    {
        _game_engine_play_multiplayer_sound(player_index, index, replicate);
        return;
    }

    /* DEVIATION: collapsed inlined copy of get_sound_length_in_ticks@0x837B6B40 (zero-xref) to its call; the inlined block (multiplayer_information.sounds table lookup + 30*runtime_maximum_play_time/1000) is field-for-field the donor body, returning 0 when any guard fails. */
    int ticks = get_sound_length_in_ticks(index);

    int count = game_engine_queued_sound_count;
    if (game_engine_queued_sound_count < 5)
    {
        mp_sound_queue[count].player_index = player_index;
        mp_sound_queue[count].index = index;
        mp_sound_queue[count].ticks_left = ticks + 5;
        mp_sound_queue[count].should_replicate = replicate;
        game_engine_queued_sound_count = count + 1;
        count = count + 1;
    }

    if (count == 1)
        _game_engine_play_multiplayer_sound(player_index, index, replicate);
}
