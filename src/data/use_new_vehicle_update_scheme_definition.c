/* use_new_vehicle_update_scheme_definition @ 0x8417F960 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char use_new_vehicle_update_scheme;

hs_global_external use_new_vehicle_update_scheme_definition =
{
    "use_new_vehicle_update_scheme",
    hs_type_boolean,
    { 0, 0 },
    &use_new_vehicle_update_scheme,
    0
};
