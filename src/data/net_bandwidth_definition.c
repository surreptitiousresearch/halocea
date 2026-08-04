/* net_bandwidth_definition @ 0x8417EF90 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int gNetMeterBandwidth;

hs_global_external net_bandwidth_definition =
{
    "net_bandwidth",
    hs_type_long_integer,
    { 0, 0 },
    &gNetMeterBandwidth,
    0
};
