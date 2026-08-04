/* network_connect_timeout_definition @ 0x8417EFA0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int gConnectionTimeoutMilliseconds;

hs_global_external network_connect_timeout_definition =
{
    "network_connect_timeout",
    hs_type_long_integer,
    { 0, 0 },
    &gConnectionTimeoutMilliseconds,
    0
};
