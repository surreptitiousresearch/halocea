/* last_damaged_object_index @ 0x84429C1C - .data zero-fill (IDA dword_84429C1C, no PDB name).
 * Full word: damage_initialize_for_new_map sets it to -1 with `stw r11,(dword_84429C1C-0x84429C18)(r8)`
 * (disasm 0x836B115C), next to the DB-named g_last_multiplayer_hit_sound_time (0x84429C18). */

int last_damaged_object_index;
