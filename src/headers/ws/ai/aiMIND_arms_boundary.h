#pragma once
#include "aiENEMY_BASE.h"
#include "aiSELENE_BASE.h"
#include "aiSELENE_IFACE.h" // canonical aiSELENE_IFACE
#include "aiSELENE.h" // canonical aiSELENE / aiSELENE_ZERO
// ws-engine ai08 — concrete "arm" types of the enemy/selene mind modules, as reached by the
// aiMIND scripted-enemy / selene transitions (a8_mind.cpp). These are next-frontier types: only the
// constructors and the vtable symbol the batched aiMIND methods take the address of are declared;
// their full layouts/bodies are the next frontier.  boundary.

struct aiBRAIN;   // ai08 — per-unit AI brain   boundary (fwd)
struct aiWATCHER; // ai08 — base watcher        boundary (fwd)

// The concrete "real" enemy-tracking arm. 0x832B6940 (??0aiENEMY@@QAA@PAVaiBRAIN@@PAVaiWATCHER@@@Z).
struct aiENEMY : aiENEMY_BASE {
    aiENEMY(aiBRAIN *pBrain, aiWATCHER *peeker); // boundary ctor
};

// aiSELENE is defined once in aiSELENE.h (included above).

// aiSELENE_IFACE is defined once (canonically) in aiSELENE_IFACE.h (included above).

// aiSELENE_ZERO is defined once in aiSELENE.h (included above).
