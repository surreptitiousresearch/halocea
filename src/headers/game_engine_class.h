#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* game_engine_class — vtable-like descriptor for the active multiplayer engine variant. Only the
 * dispose slots are reached by reconstructed callers; the remaining slots are unmodeled. Offsets
 * recovered from the DB: game_engine_dispose reads +0x8, game_engine_dispose_from_old_map reads +0x10.
 * No PDB type backs this (NO_REF); intermediate slots are kept as opaque padding to hold the layout. */

typedef struct game_engine_class
{
    unsigned char _unmodeled_0[8];     /* 0x0  — unmodeled leading slots */
    int (*dispose)(void);              /* 0x8 */
    unsigned char _unmodeled_c[4];     /* 0xC  — unmodeled slot */
    int (*dispose_from_old_map)(void); /* 0x10 */
} game_engine_class;
