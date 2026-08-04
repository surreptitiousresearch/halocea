#pragma once
// DB-verified via types_members _65873A48BD396269A883D24C03C779B2 (anonymous PDB type)
/* render_object_globals — the list of object indices found visible for the current render pass. Layout from
 * the database (count @0x00, indices @0x04). */

typedef struct render_object_globals_struct
{
    __int16 rendered_object_count;         /* 0x00 */
    char    _pad02[2];                     /* 0x02 */
    int     rendered_object_indices[256];  /* 0x04 */
} render_object_globals_struct;            /* 0x404 */

extern render_object_globals_struct render_object_globals;
