/* global_sound_data_file @ 0x844A7400 (.data, 64 bytes)
 * DB applied_types: data_file_s global_sound_data_file;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 header                     = 0x00000000000000000000000000000000
 *   +0x10 items                      = 0x00000000
 *   +0x14 max_item_count             = 0x00000000
 *   +0x18 max_names_len              = 0x00000000
 *   +0x1C cur_names_len              = 0x00000000
 *   +0x20 names                      = 0x00000000
 *   +0x24 writable                   = 0x00
 *   +0x28 hit_stats                  = 0x0000000000000000
 *   +0x30 miss_stats                 = 0x0000000000000000
 *   +0x38 file_name                  = 0x00000000
 *   +0x3C hFile                      = 0x00000000
 * the id==2 (sounds) data-file cache; data-bss zero-fill.
 * DB type data_file_s (64 bytes).
 * DEVIATION: was `data_file_s global_sound_data_file[2];`, argued from "the sound subsystem takes
 * the address of a second element at +0x40 and reads elem[1].hFile at +0x7C, and the extent to
 * data_file_dir @0x844A7480 is exactly 2*sizeof(data_file_s)". The extent is right; the owner is
 * not. The 64 bytes at +0x40 are a DIFFERENT global — its symbol is simply absent from the DB, so
 * IDA prints it as unk_844A7440, and the compiler reached it off the shared `lis/addi` base
 * because it is adjacent:
 *      data_file_get:        0x83809444  addi r3, r11, global_sound_data_file@l          ; id == 2
 *                            0x83809454  addi r3, r11, (unk_844A7440 - 0x844A7400)       ; id == 1
 *      data_file_get_handle: 0x83809F04  lwz  r3, 0x7C(r10)   ; == +0x40 base + 0x3C hFile, id == 1
 * That id==1 object is global_bitmap_data_file, which src/data/global_bitmap_data_file.c already
 * defines at 0x844A7440 — so `[2]` double-allocated it. Base-register sharing across adjacent
 * globals is not array indexing. Scalar, per the DB and per all nine consumers, which use it as
 * `&global_sound_data_file` / `global_sound_data_file.hFile`.
 */
#include "../headers/data_file_s.h"

data_file_s global_sound_data_file;
