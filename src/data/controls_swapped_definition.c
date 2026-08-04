/* controls_swapped_definition @ 0x84180AD0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char controls_swapped;

hs_global_external controls_swapped_definition =
{
    "controls_swapped",
    hs_type_boolean,
    { 0, 0 },
    &controls_swapped,
    0
};
