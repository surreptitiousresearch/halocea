#pragma once
#include <stdint.h>
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* lights_game_globals — the lighting section of the game globals tag, pointed to by lights_game_globals.
 * PARTIAL: only the render_lights master switch (first byte) is modelled; the rest is opaque. */

typedef struct lights_game_globals_struct
{
    uint8_t render_lights;   /* 0x00 — master enable for dynamic light rendering */
    /* remaining fields opaque */
} lights_game_globals_struct;

/* DB symbol lights_game_globals is untyped; canonical generated extern is void*.
 * Declare identically here (identical redeclaration is legal); consumers cast. */
extern void *lights_game_globals;
