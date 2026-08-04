// aiPLANNER::ProcessINIT @ 0x832469F0
// ?ProcessINIT@aiPLANNER@@UAAHPAX@Z
//
// One-time planner init: register the built-in behavior descriptors, load the behavior parametric
// script ("behav.ps") into bhvPSStore, init the teams subsystem, subscribe to the message system
// for the register (3) / unregister (4) lifecycle events, then chain to iaIACTOR::ProcessINIT.
//
// NOTE: the global `psSystem` symbol (?psSystem@@3PAVpsSYSTEM@@A) is a psSYSTEM*; the referencing
// code loads its pointer value and calls LoadPsFromFile on it. It is reached here through the
// established psSYSTEM_LoadPsFromFile boundary shim (see hcex_effect_names_parse_boundary.h) to
// avoid depending on ps/psSYSTEM.h's psSECTION-typed `psSystem` declaration.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/msg/msgSYSTEM.h"              // msgSystem, AddSubscriber
#include "../../headers/ws/msg/msgADDR.h"
#include "../../headers/ws/ia/iaIACTOR.h"

struct psSYSTEM;                       // ps — parametric-script system (pointer only here)
extern psSYSTEM *psSystem;             // ?psSystem@@3PAVpsSYSTEM@@A — the ps system singleton
// ?LoadPsFromFile@psSYSTEM@@QAA?AVpsSECTION@@V?$dsTSTRING@D@@00H@Z (via boundary shim) — read
// `fileName` and parse it, returning the root section by value.
extern psSECTION psSYSTEM_LoadPsFromFile(psSYSTEM *sys, dsTSTRING<char> fileName,
                                         dsTSTRING<char> name, dsTSTRING<char> errHint,
                                         int ignorePreload);

int aiPLANNER::ProcessINIT(void *pInfo)
{
    RegisterBehaviorDescs();

    // Load behav.ps relative to the ps system, with empty name / error-hint strings.
    dsTSTRING<char> errHint;  errHint.UnsafeInitEmpty();
    dsTSTRING<char> name;     name.UnsafeInitEmpty();
    dsTSTRING<char> fileName; fileName.UnsafeInit("behav.ps", -1, 0);
    *reinterpret_cast<psSECTION *>(&bhvPSStore) =
        psSYSTEM_LoadPsFromFile(psSystem, fileName, name, errHint, 0);

    reinterpret_cast<aiTEAMS_SYS *>(&teams)->ProcessINIT();

    msgSystem.AddSubscriber(reinterpret_cast<msgADDR *>(this), 3);
    msgSystem.AddSubscriber(reinterpret_cast<msgADDR *>(this), 4);

    return reinterpret_cast<iaIACTOR *>(this)->ProcessINIT(pInfo);
}
