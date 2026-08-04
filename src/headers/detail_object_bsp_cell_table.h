#pragma once
/* structure_detail_object_data — the detail-object element of structure_bsp.detail_object_data
 * (structure_bsp.h). Built once (from tag data) and then cached; structure_render_detail_objects()
 * binary-searches the `cells` block (detail_object_cell_definition entries sorted lexicographically by
 * (cell_x, cell_y, cell_z)) to find which cells fall within one cell of the camera, then uses the
 * matched entries to populate detail_object_cell_data records (detail_object_global_runtime_data.h)
 * for the renderer.
 *
 * Previously modeled as an ad-hoc flat blob ("detail_object_bsp_cell_table") reconstructed from
 * disassembly; rebound to the DB type structure_detail_object_data — the flat offsets were the
 * tag_block fields of this struct (0x00/0x04 = cells.count/.address, 0x1C = detail_objects_counts
 * .address, 0x24/0x28 = detail_object_z_reference_vectors.count/.address, 0x30 = valid). The old
 * typedef names are kept as aliases for existing consumers. */

#include "tag_block.h"
#include "detail_object_cell_definition.h"

typedef struct structure_detail_object_data
{
    tag_block       cells;                                  /* 0x00 — detail_object_cell_definition[] */
    tag_block       detail_objects;                         /* 0x0C */
    tag_block       detail_objects_counts;                  /* 0x18 — unsigned __int16[] per-(layer,cell) counts */
    tag_block       detail_object_z_reference_vectors;      /* 0x24 — real_vector4d[] (count nonzero => per-cell vectors) */
    unsigned __int8 valid;                                  /* 0x30 — bit0: view built; bit1: force rebuild */
    unsigned __int8 pad[3];                                 /* 0x31 */
    int             unused[3];                              /* 0x34 */
} structure_detail_object_data;                             /* 64 bytes */

typedef detail_object_cell_definition detail_object_bsp_cell_index;
typedef structure_detail_object_data detail_object_bsp_cell_table;
