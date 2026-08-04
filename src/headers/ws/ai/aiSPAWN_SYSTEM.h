#pragma once
#include "../ds/dsVECTOR.h"
#include "../msg/msgRES.h"
#include "aiSPAWN_FILE_INFO.h"
// ws-engine ai08: aiSPAWN_SYSTEM — the multiplayer spawn-domain system. DB inheritance chain:
// aiSPAWN_SYS_IFACE : gsDOMAIN_SYS : iaIACTOR. The base graph is a boundary here; the type is
// modelled flat with an opaque base region and the exact-offset typed fields the reversed methods
// reach (domList from the gsDOMAIN_SYS base @0xD4; isPointsLoaded from aiSPAWN_SYSTEM @0x114).
// (Byte offsets are DB-verified against the PPC image; the char-region padding documents them.)

struct gsDOMAIN;  // gs — domain base; domList elements are concretely aiDOM_SPAWN*  boundary (ptr)
struct msgADDR;   // msg — message sender address  boundary (ptr)

struct aiSPAWN_SYSTEM {
    unsigned char _base[0xD4];               // 0x000 iaIACTOR + gsDOMAIN_SYS head — boundary
    dsVECTOR<gsDOMAIN *, 8> domList;         // 0x0D4 (gsDOMAIN_SYS) active spawn domains
    unsigned char _mid[0xFC - 0xE8];         // 0x0E8 aiSPAWN_SYS_IFACE tail (domAffixes) — boundary
    dsVECTOR<aiSPAWN_FILE_INFO, 8> domainFileInfo; // 0x0FC per-domain saved spawn-point records
    int isNeedSavePoints;                    // 0x110 spawn points dirty (need re-save) flag
    int isPointsLoaded;                      // 0x114 spawn points loaded flag

    // 0x832A1A98 (?ProcessMsg@aiSPAWN_SYSTEM@@MAA?AW4msgRES@@HPAXPAVmsgADDR@@@Z) — virtual message
    // handler. REVERSED: src/ws/ai/aiSPAWN_SYSTEM__ProcessMsg.cpp.
    msgRES ProcessMsg(int msg, void *pInfo, msgADDR *pSend);
    // 0x832A2198 (?DebugCompleteActiveDomains@aiSPAWN_SYSTEM@@UAAXXZ) — mark every active spawn
    // domain completed. REVERSED: src/ws/ai/aiSPAWN_SYSTEM__DebugCompleteActiveDomains.cpp.
    void DebugCompleteActiveDomains();

    // --- boundary methods reached by the reversed handlers ---
    void LoadDomainsInfo();        // boundary
    void DestroyTempDomainInfo();  // boundary
    void SaveDomainsInfo();        // boundary
};

// Base (gsDOMAIN_SYS) message dispatch — chains an unhandled message on to the domain system.
// boundary (real symbol ?ProcessMsg@gsDOMAIN_SYS@@...); shim spelling matches the aiDOM_SPAWN pass.
msgRES gsDOMAIN_SYS_ProcessMsg(aiSPAWN_SYSTEM *self, int msg, void *pInfo, msgADDR *pSend);

// Global MP spawn-system singleton (?aiSpawnSys@@3PAVaiSPAWN_SYSTEM@@A).  boundary.
extern aiSPAWN_SYSTEM *aiSpawnSys;
