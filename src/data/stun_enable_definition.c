/* stun_enable_definition @ 0x8417F8B0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char stun_enable;

hs_global_external stun_enable_definition =
{
    "stun_enable",
    hs_type_boolean,
    { 0, 0 },
    &stun_enable,
    0
};
