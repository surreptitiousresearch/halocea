// HALO_SOUND_SYSTEM__DbgRenderSounds @0x836BD880
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/ws/rend/rendDRIVER.h"

extern rendDRIVER *rendDrv; // boundary — rend subsystem singleton (see hcex_render_present_boundary.h)

// HALO_SOUND_SYSTEM::DbgRenderSounds() @ 0x836BD890
// Debug overlay: draws a small 3D cross at the position of every currently-playing channel,
// colored green for a fully memory-resident sound-list and amber for a streamed one (disasm-
// verified: the color branch reads `channel->current.soundList->isMemory`, not a decompiler
// invention -- the pseudocode's uninitialized `v4` color local was a dropped local, reconstructed
// here from the raw byte test at +0xC on the channel's `current` slot).
void HALO_SOUND_SYSTEM::DbgRenderSounds()
{
    dbgLock.Lock(nullptr, 0);

    for (int i = 0; i < channels.nElem; ++i) {
        HALO_CHANNEL &chan = channels[i];
        if (chan.current.fsbIndex != -1) {
            m3dV pos;
            pos.x = chan.props.position.x;
            pos.y = chan.props.position.y;
            pos.z = chan.props.position.z;
            // DEVIATION: arms were transposed. 0x836BD904 `beq cr6, loc_836BD914` takes the
            // isMemory==0 path to 0x903F10 and falls through to 0x10FF10 when it is non-zero,
            // so memory-resident is the GREEN one. Caught only because this file survived the
            // linkdup drain and the copy being deleted was checked against the disassembly.
            unsigned long color = chan.current.soundList->isMemory ? 0x10FF10ul : 0x903F10ul;
            rendDrv->RenderCross(&pos, 0.1f, color);
        }
    }

    dbgLock.Unlock(nullptr, 0);
}
