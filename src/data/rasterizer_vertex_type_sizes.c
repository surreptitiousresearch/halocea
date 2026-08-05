/* rasterizer_vertex_type_sizes @0x82128514 — .rdata byte size of one vertex per
 * rasterizer_vertex_type.
 * Element width: `slwi r9, r11, 1` + `lhzx r7, r9, r8` + `extsh r3, r7` at
 * 0x837E83F8/0x837E8400/0x837E8404 (rasterizer_geometry_get_vertex_size) — a 16-bit element with
 * stride 2, matching the corpus `extern const int16_t rasterizer_vertex_type_sizes[]`.
 * Extent: 18 halfwords (0x82128514..0x82128537) cover NUMBER_OF_RASTERIZER_VERTEX_TYPES. The four
 * bytes 0x82128538..0x8212853B are zero and have no xref; the next .rdata name is the float
 * constant __real@447fe000 @0x8212853C. They are trailing pad/terminator, not a 19th and 20th
 * vertex type (no vertex type index above 17 exists), so they are not emitted.
 * Cross-check on the values: 56 = pos12+norm12+binorm12+tan12+uv8 (environment uncompressed) and
 * 68 = that plus the model node indices/weights (model uncompressed).
 */

#include <stdint.h>
#include "rasterizer_vertex_type.h"

const int16_t rasterizer_vertex_type_sizes[NUMBER_OF_RASTERIZER_VERTEX_TYPES] =
{
    56,   /* _rasterizer_vertex_type_environment_uncompressed             0x0038 */
    32,   /* _rasterizer_vertex_type_environment_compressed               0x0020 */
    20,   /* _rasterizer_vertex_type_environment_lightmap_uncompressed    0x0014 */
     8,   /* _rasterizer_vertex_type_environment_lightmap_compressed      0x0008 */
    68,   /* _rasterizer_vertex_type_model_uncompressed                   0x0044 */
    32,   /* _rasterizer_vertex_type_model_compressed                     0x0020 */
    24,   /* _rasterizer_vertex_type_dynamic_unlit                        0x0018 */
    36,   /* _rasterizer_vertex_type_dynamic_lit                          0x0024 */
    24,   /* _rasterizer_vertex_type_dynamic_screen                       0x0018 */
    16,   /* _rasterizer_vertex_type_debug                                0x0010 */
    16,   /* _rasterizer_vertex_type_decal                                0x0010 */
    20,   /* _rasterizer_vertex_type_detail_object                        0x0014 */
    32,   /* _rasterizer_vertex_type_environment_uncompressed_ff          0x0020 */
     8,   /* _rasterizer_vertex_type_environment_lightmap_uncompressed_ff 0x0008 */
    32,   /* _rasterizer_vertex_type_model_uncompressed_ff                0x0020 */
    32,   /* _rasterizer_vertex_type_model_processed                      0x0020 */
    36,   /* _rasterizer_vertex_type_unlit_zsprite                        0x0024 */
    28,   /* _rasterizer_vertex_type_widget                               0x001C */
};
