#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* ai_inactive_entity_record — one entry of the caller-provided working buffer used by
 * ai_find_inactive_encounters / ai_release_inactive_encounters (12 bytes, up to 256 records after a
 * 4-byte {count, cursor} header). No DB type exists (a raw scratch buffer); layout is defined by the
 * writer's own construction code. */

typedef struct ai_inactive_entity_record
{
    unsigned __int8 is_actor;         /* 0x00 — 1 = encounterless actor, 0 = encounter */
    unsigned char   _pad[3];          /* 0x01 */
    int             entity_index;     /* 0x04 */
    int             last_active_time; /* 0x08 */
} ai_inactive_entity_record;          /* 12 bytes */

typedef struct ai_inactive_release_memory
{
    __int16                   count;        /* 0x00 */
    __int16                   cursor;       /* 0x02 */
    ai_inactive_entity_record records[256]; /* 0x04 */
} ai_inactive_release_memory;
