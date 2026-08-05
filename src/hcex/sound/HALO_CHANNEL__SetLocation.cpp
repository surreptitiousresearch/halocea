#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/sound_location.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_CHANNEL::SetLocation(bool spatialize, const sound_location *location, float occlusion,
//                            float obstruction, bool underwater) @ 0x836BD694
//
// CAVEAT: the decompiler mis-parses the arg list as (bool, hcex_float3*, double, double, bool,
// int, bool) -- a classic float-slot-skip artifact. The disasm prologue confirms the true ABI:
// r3=this, r4=spatialize, r5=location (matches `hcex_conv_pos(location, &props.position)` using
// r5+0), f1=occlusion, f2=obstruction (each float argument also reserves a same-numbered GPR
// slot per this compiler's convention, so the trailing bool `underwater` lands in r8, not r6) --
// exactly the DB's 5-argument prototype. Reconstructed in true register order; no phantom a7/a8.
void HALO_CHANNEL::SetLocation(bool spatialize, const sound_location *location, float occlusion,
                                 float obstruction, bool underwater)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::SetLocation", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2169,
                empty_string);
        return;
    }

    this->props.is3D = spatialize;
    if (spatialize)
    {
        // CAVEAT: hcex_conv_pos/hcex_conv_vec take hcex_float3 in the original HCEX bridge
        // signature; `sound_location` (Blam's real parameter type per the DB prototype) is
        // layout-compatible (real_point3d/real_vector3d are plain {x,y,z} floats), reinterpreted
        // here field-by-field rather than via a raw cast to avoid asserting an ABI equivalence
        // the DB doesn't state.
        hcex_float3 pos = { location->position.x, location->position.y, location->position.z };
        hcex_float3 fwd = { location->forward.i, location->forward.j, location->forward.k };
        hcex_float3 vel = { location->translational_velocity.i, location->translational_velocity.j,
                             location->translational_velocity.k };
        hcex_conv_pos(&pos, &this->props.position);
        hcex_conv_vec(&fwd, &this->props.forward);
        hcex_conv_vec(&vel, &this->props.velocity);
        this->props.occlusion = occlusion;
        this->props.obstruction = obstruction;
    }

    FMOD::Channel *channel = this->current.channel;
    this->props.underwater = underwater;
    if (channel)
        this->UpdateLocation(&this->current);
    if (this->queued.channel)
        this->UpdateLocation(&this->queued);
}
