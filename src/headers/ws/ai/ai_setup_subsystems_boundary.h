#pragma once
#include "../ds/dsSHARED_PTR.h"
#include "aiWEAPON.h" // canonical aiWEAPON (concrete weapon module)
#include "aiSELENE.h" // canonical aiSELENE / aiSELENE_ZERO
#include "aiTASK.h"   // canonical aiTASK (full layout + aiTASK(aiBRAIN*) ctor)
#include "aiBEST.h"   // canonical aiBEST (full 224-byte layout + aiBEST(aiBRAIN*) ctor)
#include "aiSND.h"       // canonical aiSND (full layout + aiSND(aiBRAIN*) ctor)
#include "aiAIM_ZERO.h"  // canonical aiAIM_ZERO (full layout + aiAIM_ZERO(aiBRAIN*) ctor)
#include "aiAPPROACH.h"  // canonical aiAPPROACH (full layout + aiAPPROACH(aiBRAIN*) ctor)
#include "aiHO_SYS.h"    // canonical aiHO_SYS (full 0x20 layout + aiHO_SYS(aiBRAIN*) ctor)
// Boundary declarations for aiBRAIN::SetupBody / aiBRAIN::SetupMind (a8_init.cpp). Each concrete
// subsystem variant below derives (in the binary) from its aiXXX_BASE and is constructed with the
// owning aiBRAIN*. Here they are opaque, complete-enough-to-`new` boundary types; their real
// layouts/vtables are the next frontier. NONE of these define a competing body for a type that is
// fully defined elsewhere — they are the concrete leaf variants only.

struct aiBRAIN;

// Per-subsystem singleton "null" shared pointer. mdlGetSysSingleton<T>() returns the process-wide
// dsSHARED_PTR<T> whose pointee marks the "no module installed" sentinel.  boundary.
template<class T>
dsSHARED_PTR<T, 0, Deleter<T> > *mdlGetSysSingleton();

// ap logging (?_apLog / _apLog) — formatted diagnostic message.  boundary.
extern void _apLog(const char *fmt, ...);

// --- body subsystem variants (SetupBody) ---
#include "aiBODY.h" // canonical aiBODY (full 0x38 layout + aiBODY(aiBRAIN*) ctor)
#include "aiDIE.h"  // canonical aiDIE (full layout + aiDIE(aiBRAIN*) ctor)
struct aiBODY_ZERO  { aiBODY_ZERO(aiBRAIN *);  };  // NULL body
// aiWEAPON is defined once in aiWEAPON.h (included above).
// aiHO_SYS is defined once in aiHO_SYS.h (included above) — canonical layout + ctor.
struct aiHO_SYS_BASE_INST { aiHO_SYS_BASE_INST(); }; // plain aiHO_SYS_BASE (NULL hideout)

// --- mind subsystem variants (SetupMind) ---
// aiSND is defined once in aiSND.h (included above).
struct aiSND_ZERO   { aiSND_ZERO(aiBRAIN *);   };
// aiTASK is defined once in aiTASK.h (included above).
struct aiTASK_ZERO  { aiTASK_ZERO(aiBRAIN *);  };
#include "aiGOAL.h" // canonical aiGOAL (full layout + aiGOAL(aiBRAIN*) ctor)
struct aiGOAL_ZERO  { aiGOAL_ZERO(aiBRAIN *);  };
// aiBEST is defined once in aiBEST.h (included above).
struct aiBEST_ZERO  { aiBEST_ZERO(aiBRAIN *);  };
#include "aiPERC.h" // canonical aiPERC (full layout + aiPERC(aiBRAIN*) ctor)
struct aiPERC_ZERO  { aiPERC_ZERO(aiBRAIN *);  };
// aiISA is fully defined (aiISA_IFACE-derived, interop/social-awareness module) in aiISA.h.
#include "aiISA.h"
struct aiISA_ZERO   { aiISA_ZERO(aiBRAIN *);   };
// aiAIM_ZERO is defined once in aiAIM_ZERO.h (included above).
struct aiAIM_IDLE   { aiAIM_IDLE(aiBRAIN *);   };
struct aiAIM_BATTLE { aiAIM_BATTLE(aiBRAIN *); };
// aiAPPROACH is defined once in aiAPPROACH.h (included above).
struct aiAPPROACH_ZERO { aiAPPROACH_ZERO(aiBRAIN *); };
struct aiVISION_ZERO   { aiVISION_ZERO(aiBRAIN *);   };
struct aiVISION_IDLE   { aiVISION_IDLE(aiBRAIN *);   };
// aiVISION_BATTLE is fully defined (aiVISION_COMMON-derived) in aiVISION_BATTLE.h.
#include "aiVISION_BATTLE.h"
// aiSELENE / aiSELENE_ZERO are defined once in aiSELENE.h (included above).

// Read a packed subsystem brand-selector byte/word from the brain descriptor. The a8_init code
// indexes the descriptor as an array of dscDESC-sized (12-byte) cells; `field` picks the sub-word
// (0=__vftable, 4=refCount, 8=pBrand). Returns the low byte selector (1=NULL, 2=IDLE, 3=BATTLE,
// 4=FULL depending on the subsystem).
static inline int aiDescSubsysCode(const void *descBase, int cell, int field)
{
    return *reinterpret_cast<const int *>(
        reinterpret_cast<const char *>(descBase) + cell * 12 + field);
}

// Read a single descriptor flag byte at cell/field + byteInWord.
static inline unsigned char aiDescFlagByte(const void *descBase, int cell, int field, int byteInWord)
{
    // Big-endian target: BYTE1 = word byte index 1, HIBYTE = index 0 of the word's high side.
    return reinterpret_cast<const unsigned char *>(descBase)[cell * 12 + field + byteInWord];
}
