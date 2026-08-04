/* breakable_surfaces_definition @ 0x841800C0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char breakable_surface_effect_enabled;

hs_global_external breakable_surfaces_definition =
{
    "breakable_surfaces",
    hs_type_boolean,
    { 0, 0 },
    &breakable_surface_effect_enabled,
    0
};
