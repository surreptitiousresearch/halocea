/* net_graph_enabled_definition @ 0x8417EFB0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned __int8 gNetGraphEnabled;

hs_global_external net_graph_enabled_definition =
{
    "net_graph_enabled",
    hs_type_boolean,
    { 0, 0 },
    &gNetGraphEnabled,
    0
};
