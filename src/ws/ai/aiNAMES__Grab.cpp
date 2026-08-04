// ws-engine ai08 (a8_ext_iface.cpp): aiNAMES::Grab @0x83193C88.
// Snapshot the brain's instance/class/template display names from its animation instance,
// falling back to the shared empty string for any null name. When the brain has no instance,
// only nameInst is set (to the literal "empty inst").
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/anim/animINST.h"

extern unsigned char byte_8200155A;  // shared empty-string byte @0x8200155A

void aiNAMES::Grab(aiBRAIN *pBrain)
{
    animINST *pInst = pBrain->pInst;
    if (pInst) {
        const char *name = pInst->name;
        if (!name)
            name = reinterpret_cast<const char *>(&byte_8200155A);
        nameInst = name;

        const char *cls = pBrain->pInst->nameClass;
        if (!cls)
            cls = reinterpret_cast<const char *>(&byte_8200155A);
        nameClass = cls;

        const char *tpl = pBrain->pInst->nameTpl;
        if (!tpl)
            tpl = reinterpret_cast<const char *>(&byte_8200155A);
        nameTpl = tpl;
    } else {
        nameInst = "empty inst";
    }
}
