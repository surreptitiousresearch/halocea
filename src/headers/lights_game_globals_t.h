#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* lights_game_globals — dynamic-light runtime globals (lights.c). Only the render_lights flag (offset 0)
 * is reached by reconstructed callers. */

typedef struct lights_game_globals_t
{
    unsigned char render_lights; /* 0x0 */
} lights_game_globals_t;

/* DB symbol lights_game_globals is untyped; canonical generated extern is void*.
 * Declare identically here (identical redeclaration is legal); consumers cast. */
extern void *lights_game_globals;
