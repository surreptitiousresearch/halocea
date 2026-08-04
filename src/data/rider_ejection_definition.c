/* rider_ejection_definition @ 0x8417F8A0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern char rider_ejection;

hs_global_external rider_ejection_definition =
{
    "rider_ejection",
    hs_type_boolean,
    { 0, 0 },
    &rider_ejection,
    0
};
