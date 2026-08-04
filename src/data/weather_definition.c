/* weather_definition @ 0x84180180 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char weather;

hs_global_external weather_definition =
{
    "weather",
    hs_type_boolean,
    { 0, 0 },
    &weather,
    0
};
