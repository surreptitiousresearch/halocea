/* hcex_update_anim @0x823DE8B0 — per-frame animation tick for the HCEX bridge: advance every active
 * lip-sync state in hcexLipSyncList by `dt`, then drive the cinematic system's own animation update. */

#include "../headers/hcex/hcex_on_play_sound_boundary.h"

extern void mdlLIP_SYNCER_Update(mdlLIP_SYNCER *self, float dt);
extern void hcex_cine_update_anim(void);

void hcex_update_anim(float dt)
{
    for ( int i = 0; i < hcexLipSyncList.nElem; ++i )
        mdlLIP_SYNCER_Update(dsVECTOR_lipsync_index(&hcexLipSyncList, i)->val, dt);

    hcex_cine_update_anim();
}
