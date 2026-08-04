#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entSFX_LIST.h"
#include "../../headers/ws/ent/entSFX.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/anim/animINST.h"

// entENTITY::HideFromCamera @ 0x8252FC68 — hide this entity from the given camera slot: set the
// model instance's per-camera hide bit (state2 0x80 for camera 0, 0x100 for camera 1), then
// forward the hide to every sfx instance and to the property container.
void entENTITY::HideFromCamera(int idCamera)
{
    if (idCamera == 0)
        this->pInst->state2 |= 0x80u;
    else if (idCamera == 1)
        this->pInst->state2 |= 0x100u;

    for (int i = 0; i < this->sfxList.Length(); ++i)
    {
        entSFX *sfx = this->sfxList.Get(i);
        sfx->implHide(idCamera);
    }

    this->propContainer.HideFromCamera(idCamera);
}
