#pragma once
/* _scenery_datum — the scenery-specific runtime payload appended after the shared _object_datum inside a
 * scenery_datum. A single flags word (bit 0 = animating). Layout from the database (types_members). */

typedef struct _scenery_datum
{
    int flags;   /* 0x00 — bit 0 = animating */
} _scenery_datum; /* 4 bytes */
