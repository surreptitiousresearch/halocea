#include "../../headers/ws/ai/aiRELOADER.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h" // wpnWEAPON (with shooter@0x9C)
#include "../../headers/ws/wpn/wpnSHOOTER.h"            // wpn::SHOOTER::GetAtkAmmoClip
#include "../../headers/ws/wpn/wpnAMMO_CLIP.h"
#include "../../headers/ws/ds/dsSMART_PTR.h"

// aiRELOADER::IsLoaded() @ 0x832B3620
// ?IsLoaded@aiRELOADER@@IBA_NPBVpropWPN_AI_USABLE@@H@Z
//
// True when the controller's weapon needs no reload: no controller / no weapon / a clip-less weapon
// counts as loaded; otherwise the clip's current fill percent must exceed `perc_ammo_left`.
bool aiRELOADER::IsLoaded(const propWPN_AI_USABLE *ctrl, int perc_ammo_left) const
{
    if (!ctrl)
        return true;

    const wpnWEAPON *weapon = ctrl->weapon();
    if (!weapon)
        return true;

    // Owning smart-ptr: releases the clip reference at scope end (RAII).
    dsSMART_PTR<wpnAMMO_CLIP, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > clip =
        weapon->shooter.GetAtkAmmoClip(0);
    wpnAMMO_CLIP *pClip = clip.pointee;
    if (!pClip)
        return true;

    int percentLoaded;
    if (pClip->IsNoClip())
        percentLoaded = 100;
    else
        percentLoaded = (int)(pClip->GetAmmoClipCur() * 100.0f / pClip->GetAmmoClipMax() + 0.5f);

    return percentLoaded > perc_ammo_left;
}
