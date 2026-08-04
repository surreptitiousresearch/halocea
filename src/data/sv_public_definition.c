/* sv_public_definition @ 0x84180CC0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char sv_public_value;

hs_global_external sv_public_definition =
{
    "sv_public",
    hs_type_boolean,
    { 0, 0 },
    &sv_public_value,
    21
};
