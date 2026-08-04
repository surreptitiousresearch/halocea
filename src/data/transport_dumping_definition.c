/* transport_dumping_definition @ 0x8417EF70 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char transport_dumping;

hs_global_external transport_dumping_definition =
{
    "transport_dumping",
    hs_type_boolean,
    { 0, 0 },
    &transport_dumping,
    0
};
