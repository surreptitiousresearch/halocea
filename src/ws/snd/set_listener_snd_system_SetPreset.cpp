/* snd_system_SetPreset — out-of-line definition of the inline-only boundary helper called by
 * set_listener_properties_dsound (sound/fmod/sound_dsound_fmod.cpp). At every one of its four call
 * sites the caller performs an identical virtual dispatch through snd::SYSTEM's vtable:
 *
 *     0x836B9DD4  lwz  r3, snd::System        ; r3 = the sound-driver singleton
 *     0x836B9DD8  lwz  r8, 0(r3)              ; r8 = system->__vftable
 *     0x836B9DDC  lwz  r7, 0x44(r8)           ; r7 = vtable slot 0x44
 *     0x836B9DE0  mtctr r7
 *     0x836B9DE4  bctrl                       ; (r4 = &preset)
 *   (mirrored at 0x836B9E28/0x836B9ECC/0x836B9F0C — all slot 0x44, arg &preset in r4)
 *
 * Slot 0x44 resolves to snd::SYSTEM_FMOD::SetPreset @0x82E38CB8
 * (?SetPreset@SYSTEM_FMOD@snd@@UAAXV?$dsTSTRING@D@@@Z), the sole concrete backend, whose decompile
 * is `void SetPreset(snd::SYSTEM_FMOD *this, dsTSTRING<char> *preset)`.
 *
 * DEVIATION: the vtable slot cannot be modeled as a C++ `virtual` here because snd::SYSTEM carries a
 * hand-modeled `SYSTEM_vtbl *__vftable` member (not a compiler-generated vptr), so adding a virtual
 * would insert a second vptr and corrupt the DB-verified layout. Since SYSTEM_FMOD is the only
 * backend the dispatch can reach, this shim expresses the call as a down-cast + direct member call,
 * which is behaviourally identical to the observed slot-0x44 dispatch.
 *
 * C++ linkage — matches the caller's extern declaration (set_listener_properties_dsound.cpp:35),
 * which is a plain (non-extern-"C") C++ declaration. */

#include "headers/ws/snd/SYSTEM.h"
#include "headers/ws/snd/SYSTEM_FMOD.h"
#include "headers/ws/ds/dsTSTRING.h"

void snd_system_SetPreset(snd::SYSTEM *system, const dsTSTRING<char> &preset)
{
    /* r3 = system (as its concrete SYSTEM_FMOD), r4 = &preset; SetPreset takes the string by value
     * (V-mangle), so passing the const ref copy-constructs the callee's parameter. */
    static_cast<snd::SYSTEM_FMOD *>(system)->SetPreset(preset);
}
