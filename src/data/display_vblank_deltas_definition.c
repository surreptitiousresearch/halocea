/* display_vblank_deltas_definition @ 0x8417EF40 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char display_vblank_deltas;

hs_global_external display_vblank_deltas_definition =
{
    "display_vblank_deltas",
    hs_type_boolean,
    { 0, 0 },
    &display_vblank_deltas,
    0
};
