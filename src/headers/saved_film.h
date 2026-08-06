#pragma once
/* saved_film — demo/film record-and-playback state (save_film.c) */

#include <stdint.h>
#include <stdio.h>  /* _iobuf / FILE */
#include "player_action.h"   /* per-player action update record (32 bytes) */

typedef struct saved_film
{
    struct _iobuf *file;                  /* 0x000 */
    int16_t        mode;                  /* 0x004 — 0=idle, 1=recording, 2=playback */
    int16_t        player_count;          /* 0x006 */
    player_action  action_updates[32];    /* 0x008 */
    unsigned int   next_expected_update;  /* 0x408 */
} saved_film;

#ifdef __cplusplus
extern "C" {
#endif

extern saved_film global_saved_film;

#ifdef __cplusplus
}
#endif
