#pragma once
/* vidDEFRAG_CHUNK_OBJ_HCEX_X360 — ws-engine defrag-managed GPU buffer chunk (Xbox 360), the HCEX
 * static vertex/index buffer object. DB layout: vidDEFRAG_CHUNK base only (no own fields, 32 bytes);
 * the VID_BC_* content tag lives in the vidDEFRAG_CHUNK base at 0x1C.
 *
 * This is the flat, C-compatible stub used by the legacy .c consumers (hcex_stat_ibuf_create.c,
 * hcex_stat_vbuf_create.c, hcex_stat_vbuf_create_semi_dyn.c). The full C++ class (real
 * inheritance from vidDEFRAG_CHUNK, vtable, and the ctor/dtor/scalarDeletingDtor/operator=
 * methods actually reversed) lives at src/headers/ws/vid/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h — mirror
 * of the existing apDEFRAG_CHUNK.h (flat) / ws/ap/apDEFRAG_CHUNK.h (full) split. Both describe the
 * same DB-verified 32-byte layout. */

#include "../ws/ap/apDEFRAG_CHUNK.h" /* canonical (layout-identical); avoids C2011 vs ws/ap when co-included */

/* vidBUF_CONTENT — buffer-content kind (DB types_enum_values vidBUF_CONTENT). */
typedef enum vidBUF_CONTENT
{
    VID_BC_TEX             = 0,
    VID_BC_VERT            = 1,
    VID_BC_INDEX           = 2,
    VID_BC_VERT_LOCAL_MEM  = 3,
    VID_BC_INDEX_LOCAL_MEM = 4,
    VID_BC_TEX_MAIN_MEM    = 5,
    _VID_BC_MAX            = 6,
} vidBUF_CONTENT;

/* vidDEFRAG_CHUNK — DB: apDEFRAG_CHUNK base @0x00 (28), content @0x1C (vidBUF_CONTENT, stored as
 * 1 byte in the binary; enum-typed per the DB), 32 bytes total. Flat C twin of
 * ws/vid/vidDEFRAG_CHUNK.h. */
typedef struct vidDEFRAG_CHUNK
{
    apDEFRAG_CHUNK base;    /* 0x00 — base class */
    vidBUF_CONTENT content; /* 0x1C — VID_BC_* buffer-content tag (1 byte in-memory) */
} vidDEFRAG_CHUNK;          /* 32 bytes */

typedef struct vidDEFRAG_CHUNK_OBJ_HCEX_X360
{
    vidDEFRAG_CHUNK base; /* 0x00 — base class; no own fields */
} vidDEFRAG_CHUNK_OBJ_HCEX_X360; /* 32 bytes */
