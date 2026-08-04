/* snd_system_dbg_is_skipped — C-linkage boundary shim for the Blam C caller is_filtered_tag.
 *
 * In the binary is_filtered_tag @0x836B9F28 inlines the skip check as a direct C++ call:
 *   0x836B9F38 lis  r11, ?SystemFMod@snd@@3PAVSYSTEM_FMOD@1@A@ha    ; &snd::SystemFMod (ptr global @0x84315E1C)
 *   0x836B9F40 lwz  r11, ...@l(r11)                                 ; r11 = snd::SystemFMod (SYSTEM_FMOD*)
 *   0x836B9F44 addi r3, r11, 0x430                                  ; this = &SystemFMod->dbg  (dbg @0x430)
 *   0x836B9F48 bl   ?IsSkipped@SYSTEM_DBG_FMOD@snd@@QBA_NPBD@Z @0x82E32058
 * snd::SYSTEM_FMOD::dbg lives at offset 0x430 (DB types_members, matches SYSTEM_FMOD.h).
 *
 * is_filtered_tag.c is C and expects `extern int snd_system_dbg_is_skipped(const char *name)`,
 * so this definition uses C linkage while calling the C++ member IsSkipped internally.
 *
 * DEVIATION: the inlined 0/1 boolean normalization (clrlwi/addic/subfe @0x836B9F4C..) stays in the
 * C caller; this shim returns SYSTEM_DBG_FMOD::IsSkipped's bool result (implicitly widened to int). */

#include "headers/ws/snd/SYSTEM_FMOD.h"

namespace snd {
// ?SystemFMod@snd@@3PAVSYSTEM_FMOD@1@A @0x84315E1C — the FMOD sound-driver singleton (boundary global).
extern SYSTEM_FMOD *SystemFMod;
}

extern "C" int snd_system_dbg_is_skipped(const char *name)
{
    return snd::SystemFMod->dbg.IsSkipped(name);
}
