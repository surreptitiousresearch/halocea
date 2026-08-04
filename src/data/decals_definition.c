/* decals_definition @ 0x841800D0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char decals_enabled;

hs_global_external decals_definition =
{
    "decals",
    hs_type_boolean,
    { 0, 0 },
    &decals_enabled,
    0
};
