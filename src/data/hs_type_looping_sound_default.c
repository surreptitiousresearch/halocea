/* hs_type_looping_sound_default @ 0x82118FBC (.rdata, 4 bytes)
 * DB applied_types: const int hs_type_looping_sound_default;
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 0xFFFFFFFF
 */
#include <stdint.h>
/* hs_type_looping_sound_default @ 0x82118FBC - .rdata read-only constant. DB symbol name is the leading-underscore
 * form _hs_type_looping_sound_default (the original file-scope spelling); consumers use the plain name, the same
 * convention src/data/hs_type_string_default.c already established. Access width: lwz (disasm 0x8368DCC8); image word FFFFFFFF.
 * hs_global_reconcile_read substitutes it when an external HaloScript global's backing pointer is null. */

const int32_t hs_type_looping_sound_default = -1;
