#include "../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../headers/ws/rend/rendDRIVER.h"
#include "../headers/ws/m3d/m3dV.h"

extern rendDRIVER *rendDrv; // active render driver — boundary global

// 0x836BD880 — snd::SYSTEM_CUSTOM::DbgRenderSounds override: draw a small world-space cross at
// each live 3D channel's position (skipping unbound channels, fsbIndex == -1). Held under
// dbgLock so it doesn't race Update().
void HALO_SOUND_SYSTEM::DbgRenderSounds()
{
    this->dbgLock.Lock(nullptr, 0);

    for (int i = 0; i < this->channels.nElem; ++i)
    {
        HALO_CHANNEL *chan = &this->channels[i];
        if (chan->current.fsbIndex != -1)
        {
            m3dV pos;
            pos.x = chan->props.position.x;
            pos.y = chan->props.position.y;
            pos.z = chan->props.position.z;
            rendDrv->RenderCross(&pos, 0.1f, 0); // color arg uninitialized in the binary; reproduced as 0
        }
    }

    this->dbgLock.Unlock(nullptr, 0);
}
