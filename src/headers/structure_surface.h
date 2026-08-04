#pragma once
/* structure_surface — a single render/collision triangle in a structure BSP, identified by three
 * indices into the material's vertex buffer. */

typedef struct structure_surface
{
    unsigned short vertex_indices[3];   /* 0x00 */
} structure_surface;                    /* 6 bytes */
