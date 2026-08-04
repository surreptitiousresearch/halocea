#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md.
 * Searched DB types for 'control_to_name_entry' and any control_to_name* type: no hit (this binary
 * exposes no named type for the table element). */
/* control_to_name_entry — one entry of control_to_name_table (16 bytes): a control name string paired
 * with its game-control binding. The only reversed consumer (input_abstraction_get_game_control_byname)
 * reads .name exclusively; the trailing 12 bytes (the binding payload) are never dereferenced in
 * reversed code, so their layout stays UNRESOLVED rather than being invented. */

typedef struct control_to_name_entry
{
    const char   *name;      /* 0x00 */
    unsigned char _tail[12]; /* 0x04 — UNRESOLVED */
} control_to_name_entry;

#define CONTROL_TO_NAME_TABLE_COUNT 21

extern control_to_name_entry control_to_name_table[CONTROL_TO_NAME_TABLE_COUNT];
