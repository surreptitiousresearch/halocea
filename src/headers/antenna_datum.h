#pragma once
/* antenna_datum — one live antenna (springy aerial) instance (700 bytes). */

#include <stdint.h>
#include "real_point3d.h"
#include "antenna_vertex_datum.h"

typedef struct antenna_definition antenna_definition;   /* tag definition; accessed by raw offset */

typedef struct antenna_datum
{
    int16_t              identifier;                /* 0x00 */
    uint16_t     pad;                       /* 0x02 */
    uint8_t      initialized;               /* 0x04 */
    uint8_t      __noop;                    /* 0x05 — set = skip render (DB name kept) */
    int16_t              updates_since_last_render; /* 0x06 */
    int                  definition_index;          /* 0x08 */
    int                  object_index;              /* 0x0C */
    real_point3d         last_attachment_location;  /* 0x10 */
    antenna_vertex_datum vertices[21];              /* 0x1C — vertex/spring state (672 bytes) */
} antenna_datum;                                    /* 700 bytes */
