/* sound_gain_under_dialog_definition @ 0x8417F840 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern float sound_gain_under_dialog;

hs_global_external sound_gain_under_dialog_definition =
{
    "sound_gain_under_dialog",
    hs_type_real,
    { 0, 0 },
    &sound_gain_under_dialog,
    0
};
