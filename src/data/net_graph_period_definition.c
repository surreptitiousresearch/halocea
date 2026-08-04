/* net_graph_period_definition @ 0x8417EFC0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned int gNetGraphSamplePeriod;

hs_global_external net_graph_period_definition =
{
    "net_graph_period",
    hs_type_long_integer,
    { 0, 0 },
    &gNetGraphSamplePeriod,
    0
};
