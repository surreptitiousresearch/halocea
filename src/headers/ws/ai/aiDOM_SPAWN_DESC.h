#pragma once
// ws-engine AI spawn-domain descriptor. DB-verified layout (types_members aiDOM_SPAWN_DESC):
//   gsDOM_SPAWN_DESC base @0x0 (size 616), isSslClassInited@0x268, oppPs@0x26C — size 624 (0x270).
// The gsDOM_SPAWN_DESC base graph is a boundary here: it is modelled as an opaque span carrying
// only the ParsePS entry point aiDOM_SPAWN_DESC::ParsePS chains into. Field access on the base is
// done elsewhere through the dscDESC[] stride idiom (see aiDOM_SPAWN.cpp).

#include "../ps/psSECTION.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../DEFAULT_CTOR.h"

// gsDOM_SPAWN_DESC — base descriptor. DB-verified layout (types_members gsDOM_SPAWN_DESC,
// size 616): gsDOMAIN_DESC base@0 (192, kept opaque — its iaDESC graph is a boundary), then the
// spawn tunables below at their DB offsets.
struct gsDOM_SPAWN_DESC {
    unsigned char _base[192];   // 0x000 gsDOMAIN_DESC base (iaDESC + ssl ids + state) — boundary
    dsTSTRING<char> nameTpl;    // 0x0C0
    dsTSTRING<char> nameClass;  // 0x0C4
    dsTSTRING<char> affixStr;   // 0x0C8
    dsCONST_ARRAY<dsTSTRING<char>, 20> arrNameTpl;   // 0x0CC
    dsCONST_ARRAY<dsTSTRING<char>, 20> arrNameClass; // 0x120
    dsCONST_ARRAY<dsTSTRING<char>, 20> arrAffixes;   // 0x174
    dsCONST_ARRAY<dsTSTRING<char>, 20> arrBhv;       // 0x1C8
    float timeAfterSpawn;       // 0x21C
    float timeAfterReadySpawn;  // 0x220
    float timeOppSpawn;         // 0x224
    float timeAfterKill;        // 0x228
    float delaySpawnMin;        // 0x22C
    float delaySpawnMax;        // 0x230
    int   nSpawnMax;            // 0x234
    int   nSpawnFirst;          // 0x238
    int   nKillComplete;        // 0x23C
    int   nKillSpawn;           // 0x240
    int   nMaxSimultSameType;   // 0x244
    int   nMaxSimultSameDom;    // 0x248
    int   isIgnorePlayers;      // 0x24C
    int   isFullRandom;         // 0x250
    psSECTION psCreate;         // 0x254
    DEFAULT_CTOR<bool> isSslClassInited; // 0x258
    int   sslFuncId_Spawn;      // 0x25C
    int   sslFuncId_Reset;      // 0x260
    int   sslFuncId_SetPar;     // 0x264
    // 0x82DAB9B0 (?ParsePS@gsDOM_SPAWN_DESC@@UAAXVpsSECTION@@ABV?$dsTSTRING@D@@@Z) — base parse.
    void ParsePS(psSECTION psRead, const dsTSTRING<char> &hintErr);
    // 0x828404B0 (?InitSslClass@gsDOMAIN_DESC@@UAAHABV?$dsTSTRING@D@@@Z) — base (gsDOMAIN_DESC)
    // script-class init; returns non-zero on success.  boundary.
    int InitSslClass(const dsTSTRING<char> &hintErr);
    // virtual (?UnshareSslClass@...@@UAAX_N0@Z) — force this descriptor's script class to be a
    // private (unshared) copy.  boundary.
    void UnshareSslClass(bool a, bool b);
};

struct aiDOM_SPAWN_DESC : gsDOM_SPAWN_DESC {
    DEFAULT_CTOR<bool> isSslClassInited; // 0x268 (DB: DEFAULT_CTOR<bool>) — has been script-inited
    // padding to 0x26C
    psSECTION oppPs;            // 0x26C opponent-spawn parse section

    // 0x83263F00 (?ParsePS@aiDOM_SPAWN_DESC@@UAAXVpsSECTION@@ABV?$dsTSTRING@D@@@Z) — parse the base
    // descriptor, then capture the nested "OPP_PS" sub-section into oppPs. REVERSED.
    void ParsePS(psSECTION psRead, const dsTSTRING<char> &hintErr);

    // 0x832A1EB8 (?InitSslClass@aiDOM_SPAWN_DESC@@UAAHABV?$dsTSTRING@D@@@Z) — override: base init,
    // then one-time UnshareSslClass. REVERSED: src/ws/ai/aiDOM_SPAWN_DESC__InitSslClass.cpp.
    int InitSslClass(const dsTSTRING<char> &hintErr);
};
