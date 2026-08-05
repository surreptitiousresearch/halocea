/* game_engine_queued_sound_count @ 0x844A5C90 - .data zero-fill (IDA dword_844A5C90, no PDB name).
 * Full word: game_engine_intialize_queued_sounds primes it to 1 with `stw r11,(dword_844A5C90-0x844A5C40)
 * (r30)` (disasm 0x837B6CEC), r30 anchored on mp_sound_queue. */

int game_engine_queued_sound_count;
