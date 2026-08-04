#pragma once
/* flag_datum — per-instance runtime state for a placed "flag" (cloth simulation) object (5820 bytes,
 * DB layout). */

#include "real_point3d.h"
#include "flag_vertex_datum.h"
#include "flag_cell_datum.h"

typedef struct flag_datum
{
    __int16            identifier;                  /* 0x00 */
    unsigned __int8    __noop;                       /* 0x02 */
    unsigned __int8    initialized;                   /* 0x03 */
    unsigned __int8    update_state;                  /* 0x04 */
    unsigned __int8    _pad05[1];
    __int16            updates_since_last_render;     /* 0x06 */
    int                object_index;                  /* 0x08 */
    int                definition_index;               /* 0x0C */
    real_point3d       first_attachment;               /* 0x10 */
    flag_vertex_datum  vertices[225];                   /* 0x1C */
    flag_cell_datum    cells[196];                      /* 0x1534 */
} flag_datum;                                            /* 5820 bytes */
