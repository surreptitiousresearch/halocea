/* sound_gain_under_dialog_definition @ 0x8417F840 (.data, 16 bytes)
 * DB applied_types: hs_global_external sound_gain_under_dialog_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821211F8 -> "sound_gain_under_dialog"
 *   +0x04 type                       = 0x0006
 *   +0x08 pointer                    = 0x84177D34 -> sound_gain_under_dialog
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern float sound_gain_under_dialog;

hs_global_external sound_gain_under_dialog_definition =
{
    "sound_gain_under_dialog",  /* name */
    hs_type_real,               /* type */
    { 0, 0 },                   /* _pad06.._pad07 */
    &sound_gain_under_dialog,   /* pointer */
    0,                          /* console_flags */
};
