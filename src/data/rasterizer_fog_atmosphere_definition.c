/* rasterizer_fog_atmosphere_definition @ 0x84180C30 (.data, 16 bytes)
 * DB applied_types: hs_global_external rasterizer_fog_atmosphere_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211EFD0 -> "rasterizer_fog_atmosphere"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x84175A07 -> rasterizer_debug_options+0x27
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/rasterizer_debug_options.h"
extern rasterizer_debug_options_struct rasterizer_debug_options;

hs_global_external rasterizer_fog_atmosphere_definition =
{
    "rasterizer_fog_atmosphere",             /* name */
    hs_type_boolean,                         /* type */
    { 0, 0 },                                /* _pad06.._pad07 */
    (char *)&rasterizer_debug_options + 39,  /* pointer */
    0,                                       /* console_flags */
};
