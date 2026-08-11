/* set_listener_properties_dsound @0x836B9B68 — push this frame's 3D listener transform into the FMOD
 * (DirectSound) backend. Copies position / translational velocity / forward / up out of the platform
 * listener properties into haloSoundSystem->listenerProps (each guarded by a finite-value strong assert),
 * rebuilds the listener matrix from the Halo-basis matrix, re-derives the listener position by transforming
 * the origin through that matrix, then selects the FMOD reverb preset: "Underwater" when submerged, else
 * the preset mapped from the current sound_environment tag's name via EAX_MAP (falling back to "Off" when
 * there is no environment tag and "Generic" when the tag name matches no EAX_MAP entry).
 *
 * C++ translation unit: the function lives in sound/fmod/sound_dsound_fmod.cpp and operates on the C++
 * HALO_SOUND_SYSTEM / snd::SYSTEM / dsTSTRING graph, so it cannot be expressed as plain C.
 *
 * DEVIATION: the four debug finite-value checks (STRONG_ASSERT2_HELPER::asserd<m3dV>) are funneled through
 * a single declared boundary helper rather than reproducing the template's exact instantiation. */

#include <stdint.h>
#include "headers/platform_sound_listener_properties.h"
#include "headers/hcex/hcex_matr4x3.h"
#include "headers/hcex/HALO_SOUND_SYSTEM.h"
#include "headers/ws/m3d/m3dV.h"
#include "headers/ws/m3d/m3dMATR.h"
#include "headers/ws/snd/SYSTEM.h"
#include "headers/ws/ds/dsTSTRING.h"
#include "headers/EAX_MAP.h"

/* boundary declarations (defined elsewhere / debug scaffolding) */
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern const m3dV m3dVZero;
extern bool m3dCheckFinite(const m3dV *v);
extern void STRONG_ASSERT2_HELPER_asserd_m3dV(const char *expr, const char *file, int line,
    const char *name, const m3dV *value);
extern void hcex_make_matr(const hcex_matr4x3 *in, m3dMATR *out);
extern "C" char *tag_get_name(int tag_index);
/* SetPreset is a snd::SYSTEM vtable slot (offset 0x44) owned by the src/ws/snd drain; called here through
 * a boundary wrapper so this TU need not redeclare the shared SYSTEM vtable. */
extern void snd_system_SetPreset(snd::SYSTEM *system, const dsTSTRING<char> &preset);

static const char kSourceFile[] = "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp";

extern "C" void set_listener_properties_dsound(const platform_sound_listener_properties *properties)
{
    m3dV position;
    position.x = properties->position.x;
    position.y = properties->position.y;
    position.z = properties->position.z;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&position))
        STRONG_ASSERT2_HELPER_asserd_m3dV("m3dCheckFinite(__tmp__)", kSourceFile, 2567, "__tmp__", &position);
    haloSoundSystem->listenerProps.pos = position;

    m3dV velocity;
    velocity.x = properties->translational_velocity.i;
    velocity.y = properties->translational_velocity.j;
    velocity.z = properties->translational_velocity.k;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&velocity))
        STRONG_ASSERT2_HELPER_asserd_m3dV("m3dCheckFinite(__tmp__)", kSourceFile, 2568, "__tmp__", &velocity);
    haloSoundSystem->listenerProps.vel = velocity;

    m3dV forward;
    forward.x = properties->forward.i;
    forward.y = properties->forward.j;
    forward.z = properties->forward.k;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&forward))
        STRONG_ASSERT2_HELPER_asserd_m3dV("m3dCheckFinite(__tmp__)", kSourceFile, 2569, "__tmp__", &forward);
    haloSoundSystem->listenerProps.forw = forward;

    m3dV up;
    up.x = properties->up.i;
    up.y = properties->up.j;
    up.z = properties->up.k;
    if (!IGNORE_STRONG_ASSERT && !m3dCheckFinite(&up))
        STRONG_ASSERT2_HELPER_asserd_m3dV("m3dCheckFinite(__tmp__)", kSourceFile, 2570, "__tmp__", &up);
    haloSoundSystem->listenerProps.up = up;

    hcex_make_matr((const hcex_matr4x3 *)&properties->matrix, &haloSoundSystem->listenerProps.matr);

    /* Re-derive the listener position as the world-space image of the matrix origin. */
    m3dV origin = m3dVZero;
    haloSoundSystem->listenerProps.matr.TransformPoint(&origin, &origin);
    haloSoundSystem->listenerProps.pos = origin;

    dsTSTRING<char> preset;
    preset.pBuffer = 0;

    if (properties->underwater)
    {
        preset.UnsafeInit("Underwater", -1, 0);
        snd_system_SetPreset(snd::System, preset);
    }
    else if (properties->sound_environment_tag == -1)
    {
        preset.UnsafeInit("Off", -1, 0);
        snd_system_SetPreset(snd::System, preset);
    }
    else
    {
        const char *environment_name = tag_get_name(properties->sound_environment_tag);
        bool matched = false;

        for (unsigned int index = 0; index < 23; ++index)
        {
            const char *candidate = EAX_MAP[index].tag_name;
            const char *name = environment_name;
            int difference;
            do
            {
                int c = (unsigned char)*name;
                difference = c - (unsigned char)*candidate;
                if (c == 0)
                    break;
                ++name;
                ++candidate;
            } while (difference == 0);

            if (difference == 0)
            {
                matched = true;
                preset.pBuffer = EAX_MAP[index].fmod_preset.pBuffer;   /* raw buffer-pointer copy, then a manual refCount bump */
                ++preset.pBuffer->refCount;
                snd_system_SetPreset(snd::System, preset);
                break;
            }
        }

        if (!matched)
        {
            preset.pBuffer = 0;
            preset.UnsafeInit("Generic", -1, 0);
            snd_system_SetPreset(snd::System, preset);
        }
    }
}
