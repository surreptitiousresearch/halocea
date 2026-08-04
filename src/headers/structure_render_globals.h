#pragma once
// DB-verified via types_members _4E88B04EF17AC3A2E2A920A3B1F10436 (anonymous PDB type)
/* structure_render_globals — transient per-frame structure-render state. Fully resolved from the database
 * anonymous type (_4E88B04EF17AC3A2E2A920A3B1F10436) backing the global. */

#include "real_vector3d.h"

typedef struct structure_render_globals_data
{
    unsigned __int8 environment_geometry_valid;   /* 0x00 */
    char            _pad01[3];                     /* 0x01 */
    int             environment_triangles_index;   /* 0x04 */
    unsigned __int8 fog_offset_valid;             /* 0x08 */
    char            _pad09[3];                     /* 0x09 */
    real_vector3d   fog_offset_vector;            /* 0x0C */
} structure_render_globals_data;                  /* 24 bytes */

extern structure_render_globals_data structure_render_globals;
