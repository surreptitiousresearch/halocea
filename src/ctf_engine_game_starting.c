/* ctf_engine_game_starting @0x83806410 — tail-call forwarder (`li r4, 0; li r3, 0x16; b
 * game_engine_play_multiplayer_sound`; confirmed via disasm, not a bare blr). Plays multiplayer sound
 * _multiplayer_sound_ctf (0x16, the CTF game-starting cue) without replication. */

#include <stdint.h>
#include "headers/game_engine_multiplayer_sound.h"

extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);

void ctf_engine_game_starting(void)
{
    game_engine_play_multiplayer_sound(_multiplayer_sound_ctf, 0);
}
