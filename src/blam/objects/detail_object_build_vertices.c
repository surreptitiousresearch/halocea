/* detail_object_build_vertices @0x83712C28 — expand a run of packed 6-byte `detail_object` records for one
 * cell into 6 world-space `detail_object_vertex` records each (a non-indexed 2-triangle quad, corner order
 * 0,1,2,0,2,3), writing them contiguously into `detail_object_vertices`.
 *
 * World position: x/y come from the cell's integer coordinate * 8 plus the object's 8-bit fixed-point
 * cell-local fraction (`position[0..1]`) scaled by 1/255 * 8; z comes from the cell's z plus the fraction
 * vector dotted against the cell's local surface reference plane (`z_reference_vector`), all *8 — an 8-unit
 * grid cell with an 8-bit sub-cell fraction, height following the local surface plane.
 *
 * DEVIATION: the DB's own decompiler output for this function has a corrupted signature — it shows only 4
 * parameters (collection_definition, detail_objects, and two locals it mislabels `detail_object_count` and
 * `cell`, both typed __int64) with `detail_object_vertices` missing entirely. Traced via disasm: r3/r4/r6/r7
 * are collection_definition/detail_objects/detail_object_count/cell exactly as the DB's 5-arg prototype
 * says; r5 (detail_object_vertices) — never referenced in the garbled decompilation — is in fact used
 * throughout as the output cursor. The floating-point arithmetic and the color/type bit-packing below are
 * otherwise faithful to the decompiler; only the pointer/parameter identities were wrong.
 * DEVIATION: `vertex.color`'s bit-scramble (color_bits) is reproduced verbatim from the decompiler/disasm;
 * it is some 16→32-bit color channel expansion or dither pattern on the source `detail_object.color` field
 * that was not further reverse engineered. */

#include "headers/detail_object_collection_definition.h"
#include "headers/detail_object.h"
#include "headers/detail_object_vertex.h"
#include "headers/detail_object_type_definition.h"
#include "headers/detail_object_global_runtime_data.h"

void detail_object_build_vertices(const detail_object_collection_definition *collection_definition,
                                  const detail_object *detail_objects,
                                  detail_object_vertex *detail_object_vertices,
                                  int detail_object_count, detail_object_cell_data *cell)
{
    static const int corner_index_delta[6] = {0, 1, 2, 0, 2, 3};

    const real_vector4d *zref = cell->z_reference_vector;

    for (int i = 0; i < detail_object_count; ++i)
    {
        const detail_object *object = &detail_objects[i];

        float fraction_x = object->position[0] * 0.0039215689f; /* / 255 */
        float fraction_y = object->position[1] * 0.0039215689f;
        float fraction_z = object->position[2] * 0.0039215689f;

        detail_object_vertex vertex;
        vertex.position.x = 8.0f * ((float)cell->cell_x + fraction_x);
        vertex.position.y = 8.0f * ((float)cell->cell_y + fraction_y);
        vertex.position.z = 8.0f * (cell->cell_z
                                    + (zref->n[0] * fraction_x + zref->n[1] * fraction_y + zref->n[2] * fraction_z)
                                    + zref->n[3]);

        unsigned int color_bits = object->color;
        vertex.color = (8 * (((color_bits << 5) | (color_bits >> 27)) & 0xFFFF001F | color_bits & 0xFFFFE01F
                             | (4 * color_bits) & 0x1F80 | 0xFFE00000))
                     | (((color_bits >> 1) & 0xE | color_bits & 0xFFFFFFF1) >> 1) & 0x307;

        unsigned int type_index = (unsigned char)((object->data >> 4) % collection_definition->type_definitions.count);
        unsigned int frame_selector = object->data & 0xF;

        const detail_object_type_definition *type_def =
            (const detail_object_type_definition *)collection_definition->type_definitions.address + type_index;

        unsigned int frame_index = frame_selector % type_def->frame_count + type_def->first_frame_index;
        unsigned int data_base = (((frame_index << 8) & 0xFF00) | 0x10000 | (unsigned char)type_index) << 8;

        for (int corner = 0; corner < 6; ++corner)
        {
            detail_object_vertex *out = &detail_object_vertices[6 * i + corner];
            *out = vertex;
            out->data = data_base + corner_index_delta[corner];
        }
    }
}
