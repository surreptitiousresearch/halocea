/* loud_dialog_hack_definition @ 0x8417F830 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char loud_dialog_hack;

hs_global_external loud_dialog_hack_definition =
{
    "loud_dialog_hack",
    hs_type_boolean,
    { 0, 0 },
    &loud_dialog_hack,
    0
};
