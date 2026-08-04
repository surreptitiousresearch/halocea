#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entENTITY_showhide_boundary.h"
#include "../../headers/ws/ent/entDESC.h"
#include "../../headers/ws/ia/iaDESC.h"
#include "../../headers/ws/sml/STATE.h"
#include "../../headers/ws/ent/entSFX_LIST.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/anim/animINST.h"

// entENTITY::implHide @ 0x826A88B0 — hide this entity (target of hcex flattened extern
// entENTITY_implHide). No-op if the 0x1000 "hidden" flag is already latched. Otherwise: if the
// entity is in its START (active) sml state, latch a 0x2000000 "was-active" flag and clear START;
// then swap sml SHOW->HIDE, mark the model instance hidden, pause+hide the sfx list, pause the
// playing-sound list (unless the class descriptor's own state bit 0 says otherwise), latch the
// hidden flag and hide the property container.
void entENTITY::implHide()
{
    if ((this->stateEnt.val & 0x1000) != 0)
        return;

    entDESC *desc = Desc(this);
    if (this->stateSml.Is(desc->GetState_START()))
    {
        this->stateEnt.val |= 0x2000000;
        this->stateSml.Clear(Desc(this)->GetState_START());
    }

    this->stateSml.Clear(Desc(this)->GetState_SHOW());
    this->stateSml.Set(Desc(this)->GetState_HIDE());

    this->pInst->state |= 1;
    this->sfxList.Pause();
    this->sfxList.Hide();

    entDESC *d = Desc(this);
    if (!d || (d->state.state & 1) == 0)
        this->snd.playList.Pause(1);

    this->stateEnt.val |= 0x1000;
    this->propContainer.Hide();
}
