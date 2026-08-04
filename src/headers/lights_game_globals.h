#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* lights_game_globals — the lighting section of the game globals tag, pointed to by lights_game_globals.
 * PARTIAL: only the render_lights master switch (first byte) is modelled; the rest is opaque. */

typedef struct lights_game_globals_struct
{
    unsigned __int8 render_lights;   /* 0x00 — master enable for dynamic light rendering */
    /* remaining fields opaque */
} lights_game_globals_struct;

/* DB symbol lights_game_globals is untyped; canonical generated extern is void*.
 * Declare identically here (identical redeclaration is legal); consumers cast. */
extern void *lights_game_globals;
