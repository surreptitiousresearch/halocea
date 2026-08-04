/* global_sound_data_file @ 0x844A7400 (128 bytes) — data-bss zero-fill. DB applied-type is a single
 * `data_file_s` (64), but the sound subsystem (0x83809xxx) accesses TWO elements off the one base
 * (global_sound_data_file@l / 0x844A7400): elem[1] base at +0x40 (address taken in the same functions
 * that take elem[0]) and elem[1].hFile at +0x7C (== 0x40 + struct-offset 0x3C of hFile). The extent to
 * the next symbol (data_file_dir @0x844A7480) is exactly 2*sizeof(data_file_s). It is an array of two. */
#include "../headers/data_file_s.h"

data_file_s global_sound_data_file[2];
