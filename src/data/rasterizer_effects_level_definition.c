/* rasterizer_effects_level_definition @ 0x8417F5B0 (.data, 16 bytes)
 * DB applied_types: hs_global_external rasterizer_effects_level_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821215C8 -> "rasterizer_effects_level"
 *   +0x04 type                       = 0x0007
 *   +0x08 pointer                    = 0x84175A4C -> "" (empty string at target)
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/rasterizer_debug_options.h"
extern rasterizer_debug_options_struct rasterizer_debug_options;

hs_global_external rasterizer_effects_level_definition =
{
    "rasterizer_effects_level",               /* name */
    hs_type_short_integer,                    /* type */
    { 0, 0 },                                 /* _pad06.._pad07 */
    (char *)&rasterizer_debug_options + 108,  /* pointer */
    0,                                        /* console_flags */
};
