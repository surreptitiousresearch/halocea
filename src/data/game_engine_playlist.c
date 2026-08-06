/* game_engine_playlist @ 0x8417D9C4 (.data, 16 bytes)
 * DB applied_types: game_engine_playlist_s game_engine_playlist;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 playlist                   = 0x00000000
 *   +0x04 playlist_count             = 0x00000000
 *   +0x08 playlist_max_count         = 0x00000000
 *   +0x0C now_playing                = 0xFFFFFFFF
 * DEVIATION: previously spelled as an uninitialised definition and described as "data-bss
 * zero-fill". The .data record is not zero — `now_playing` is 0xFFFFFFFF, the Blam -1 "none"
 * sentinel — so a tentative definition handed the engine index 0 (a valid playlist entry) where
 * the loader supplies "nothing playing".
 * Caught 2026-08-06 by data_provenance.py --verify against the .data record. */
#include "../headers/game_engine_playlist_s.h"

game_engine_playlist_s game_engine_playlist = {
    0,   /* playlist */
    0,   /* playlist_count */
    0,   /* playlist_max_count */
    -1,  /* now_playing */
};
