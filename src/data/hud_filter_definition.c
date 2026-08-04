/* hud_filter_definition @ 0x84180C00 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char always_filter_hud;

hs_global_external hud_filter_definition =
{
    "hud_filter",
    hs_type_boolean,
    { 0, 0 },
    &always_filter_hud,
    0
};
