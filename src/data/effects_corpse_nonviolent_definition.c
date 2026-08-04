/* effects_corpse_nonviolent_definition @ 0x8417F7C0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char effects_corpse_nonviolent;

hs_global_external effects_corpse_nonviolent_definition =
{
    "effects_corpse_nonviolent",
    hs_type_boolean,
    { 0, 0 },
    &effects_corpse_nonviolent,
    0
};
