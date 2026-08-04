#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_props_boundary.h"
#include "../../headers/ws/ctrl/ctrlPROP_LIST.h"
#include "../../headers/ws/ds/dsBIT_ARRAY_IMPL.h"

// aiWATCHER::TranslateSTRID_FLAGS @ 0x8328E420
// ?TranslateSTRID_FLAGS@aiWATCHER@@IAAXABUctrlPROP_LIST@@@Z
// Repack the tracked body's property/state bit-cache (`propCache`) into this watcher's compact
// AI_FSM_PROPS flag word. Each source property bit maps to a fixed AI_FSM_PROPS bit.
void aiWATCHER::TranslateSTRID_FLAGS(const ctrlPROP_LIST *propCache)
{
    const unsigned int *bits = propCache->props.array;

    fsmPropFlags.val = 0;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_MINIZOOM.id, bits, 12))          fsmPropFlags.val |= 0x1;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_REACHED_MZ.id, bits, 12))        fsmPropFlags.val |= 0x2;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_WPN_SHOOTING.id, bits, 12))      fsmPropFlags.val |= 0x4;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_FLOOR_HO_MINIZOOM.id, bits, 12)) fsmPropFlags.val |= 0x10;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_WLN_HO_MINIZOOM.id, bits, 12))   fsmPropFlags.val |= 0x20;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_UNCONTROLLED.id, bits, 12))      fsmPropFlags.val |= 0x40;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_FALLING.id, bits, 12))           fsmPropFlags.val |= 0x80;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_HIDEOUT.id, bits, 12))           fsmPropFlags.val |= 0x100;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_REACHED_COVER.id, bits, 12))     fsmPropFlags.val |= 0x200;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_HIDEOUT_LEAVE.id, bits, 12))     fsmPropFlags.val |= 0x400;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_SMART_OBJ.id, bits, 12))         fsmPropFlags.val |= 0x800;
    if (dsBIT_ARRAY_IMPL::Is(HO_POS_LOWHO.id, bits, 12))                  fsmPropFlags.val |= 0x1000;
    if (dsBIT_ARRAY_IMPL::Is(HO_POS_LEFT.id, bits, 12))                   fsmPropFlags.val |= 0x2000;
    if (dsBIT_ARRAY_IMPL::Is(HO_POS_RIGHT.id, bits, 12))                  fsmPropFlags.val |= 0x4000;
    if (dsBIT_ARRAY_IMPL::Is(HO_POS_OUTER.id, bits, 12))                  fsmPropFlags.val |= 0x8000;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_UNCONSCIOUS.id, bits, 12))       fsmPropFlags.val |= 0x10000;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_LOW_G.id, bits, 12))             fsmPropFlags.val |= 0x20000;
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_WLN.id, bits, 12))               fsmPropFlags.val |= 0x40000;
    if (dsBIT_ARRAY_IMPL::Is(MDL_PROP_ST_WLN_JUMP_FLY.id, bits, 12))      fsmPropFlags.val |= 0x80000;
}
