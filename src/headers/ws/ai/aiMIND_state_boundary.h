#pragma once
// Boundary declarations for the concrete enemy/vision leaf variants constructed by the aiMIND
// enemy/vision/isa state-transition methods (a8_mind.cpp: ZeroEnemy / FreezeEnemy /
// DecideAppropriateVision). In the binary each derives from its aiXXX_BASE and is built with the
// owning aiBRAIN*; here they are opaque, complete-enough-to-`new` boundary types (their real
// layouts/vtables live in headers_ref and are the next frontier). These declare no competing body
// for any type fully defined elsewhere.
//
// Note on the vtable-repoint idiom: the binary constructs the parent variant (aiVISION_BATTLE /
// aiISA_IFACE) then overwrites __vftable with the leaf's (aiVISION_CHEAP / aiISA_ZERO). The source
// models that as a direct `new <leaf>(brain)`, which is the semantically-equivalent leaf ctor.

struct aiBRAIN;

struct aiENEMY_ZERO   { aiENEMY_ZERO(aiBRAIN *);   };  // null "no enemy" arm
struct aiENEMY_FROZEN { aiENEMY_FROZEN(aiBRAIN *); };  // "enemy frozen / realizing death" arm
// aiVISION_CHEAP is fully defined (aiVISION_BATTLE-derived) in aiVISION_CHEAP.h.
#include "aiVISION_CHEAP.h"
