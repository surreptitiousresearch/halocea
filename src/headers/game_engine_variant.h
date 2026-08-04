#pragma once
/* game_engine_variant — the engine-specific portion of a game variant, a union of per-mode settings
 * (24 bytes, sized by the oddball view; DB layout / member order). */

#include "ctf_variant.h"
#include "slayer_variant.h"
#include "king_variant.h"
#include "oddball_variant.h"
#include "race_variant.h"
#include "terminator_variant.h"

typedef union game_engine_variant
{
    ctf_variant        ctf;        /* 8 bytes  */
    slayer_variant     slayer;     /* 3 bytes  */
    king_variant       king;       /* 1 byte   */
    oddball_variant    oddball;    /* 24 bytes — sizes the union */
    race_variant       race;       /* 8 bytes  */
    terminator_variant terminator; /* 4 bytes  */
} game_engine_variant;  /* 24 bytes */
