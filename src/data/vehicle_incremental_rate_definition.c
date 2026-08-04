/* vehicle_incremental_rate_definition @ 0x84180190 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int vehicle_incremental_rate;

hs_global_external vehicle_incremental_rate_definition =
{
    "vehicle_incremental_rate",
    hs_type_long_integer,
    { 0, 0 },
    &vehicle_incremental_rate,
    0
};
