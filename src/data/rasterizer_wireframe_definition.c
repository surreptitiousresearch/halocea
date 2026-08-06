/* rasterizer_wireframe_definition @ 0x84180C50 (.data, 16 bytes)
 * DB applied_types: hs_global_external rasterizer_wireframe_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211EFA0 -> "rasterizer_wireframe"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x841759E6 -> "" (empty string at target)
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/rasterizer_debug_options.h"
extern rasterizer_debug_options_struct rasterizer_debug_options;

hs_global_external rasterizer_wireframe_definition =
{
    "rasterizer_wireframe",                 /* name */
    hs_type_boolean,                        /* type */
    { 0, 0 },                               /* _pad06.._pad07 */
    (char *)&rasterizer_debug_options + 6,  /* pointer */
    0,                                      /* console_flags */
};
