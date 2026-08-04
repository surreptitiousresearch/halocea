#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entENTITY_showhide_boundary.h"
#include "../../headers/ws/ent/entDESC.h"
#include "../../headers/ws/ia/iaDESC.h"
#include "../../headers/ws/sml/STATE.h"
#include "../../headers/ws/ent/entSFX_LIST.h"
#include "../../headers/ws/ent/entFOLLOW.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/anim/animINST.h"

// entENTITY::implShow @ 0x826A8A80 — show this entity (target of hcex flattened extern
// entENTITY_implShow). Only acts when the 0x1000 "hidden" flag is currently latched. Restores the
// START sml state if the 0x2000000 "was-active" flag was set, swaps sml HIDE->SHOW, un-hides the
// model instance, refreshes the follow pose, clears the hidden flag, resumes+shows the sfx list
// and the playing-sound list, and shows the property container.
void entENTITY::implShow()
{
    if ((this->stateEnt.val & 0x1000) == 0)
        return;

    if ((this->stateEnt.val & 0x2000000) != 0)
    {
        this->stateEnt.val &= ~0x2000000u;
        this->stateSml.Set(Desc(this)->GetState_START());
    }

    this->stateSml.Clear(Desc(this)->GetState_HIDE());
    this->stateSml.Set(Desc(this)->GetState_SHOW());

    this->pInst->state &= ~1u;
    this->followObj.UpdateFollow();
    this->stateEnt.val &= ~0x1000u;
    this->sfxList.Resume();
    this->sfxList.Show();
    this->snd.playList.Resume();
    this->propContainer.Show();
}
