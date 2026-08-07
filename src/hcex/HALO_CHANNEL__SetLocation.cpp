#include "../headers/hcex/HALO_CHANNEL_boundary.h"

// HALO_CHANNEL::SetLocation @ 0x836BD640 — update props' 3D position/forward/velocity (converting
// Blam sound_location vectors to m3d space) plus occlusion/obstruction/underwater and the
// spatialize (is3D) flag, then push them to whichever of current/queued has a live FMOD channel
// via UpdateLocation.
//
// The Blam sound_location's leading position/forward/translational_velocity are three contiguous
// 3-float vectors, which is exactly hcex_conv_pos/hcex_conv_vec's hcex_float3 input; the raw
// decompile reflects this by indexing `location`/`location+1`/`location+2`.
//
// CAVEAT: hcex_conv_pos/hcex_conv_vec take hcex_float3 in the original HCEX bridge signature;
// sound_location's real_point3d/real_vector3d are layout-identical plain {x,y,z} floats, copied
// field-by-field here rather than via a raw pointer cast so no ABI equivalence the DB does not
// state is asserted. Same treatment as the sibling reconstruction in src/hcex/sound/.
void HALO_CHANNEL::SetLocation(bool spatialize, const sound_location *location,
                              float occlusion, float obstruction, bool underwater)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::SetLocation", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2169, empty_string);
        return;
    }

    this->props.is3D = spatialize;
    if (spatialize)
    {
        hcex_float3 position = { location->position.x, location->position.y, location->position.z };
        hcex_float3 forward  = { location->forward.i, location->forward.j, location->forward.k };
        hcex_float3 velocity = { location->translational_velocity.i,
                                 location->translational_velocity.j,
                                 location->translational_velocity.k };
        hcex_conv_pos(&position, &this->props.position);
        hcex_conv_vec(&forward, &this->props.forward);
        hcex_conv_vec(&velocity, &this->props.velocity);
        this->props.occlusion = occlusion;
        this->props.obstruction = obstruction;
    }

    this->props.underwater = underwater;

    if (this->current.channel)
        UpdateLocation(&this->current);
    if (this->queued.channel)
        UpdateLocation(&this->queued);
}
