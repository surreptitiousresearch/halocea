/* rasterizer_fps_definition @ 0x84180C60 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/rasterizer_debug_options.h"
extern rasterizer_debug_options_struct rasterizer_debug_options;

hs_global_external rasterizer_fps_definition =
{
    "rasterizer_fps",
    hs_type_boolean,
    { 0, 0 },
    &rasterizer_debug_options,
    95
};
