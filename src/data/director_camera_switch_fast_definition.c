/* director_camera_switch_fast_definition @ 0x84180B50 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include <stdint.h>
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern uint8_t director_camera_switch_fast;

hs_global_external director_camera_switch_fast_definition =
{
    "director_camera_switch_fast",
    hs_type_boolean,
    { 0, 0 },
    &director_camera_switch_fast,
    0
};
