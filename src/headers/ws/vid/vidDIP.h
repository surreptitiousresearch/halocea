#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsNAME_CMP.h"
#include "../os/osLOCK.h"
#include "../os/osTIMER.h" // vidDIP_STAT::timer -- os subsystem type, canonical home
// ws-engine vid: per-frame draw-call ("DIP" = DrawIndexedPrimitive) statistics, organised as a
// 3-level named hierarchy (group -> class -> individual dip), each level carrying a running
// dip/poly counter. Reached via vidDriver->dipStat. DB-verified layouts (types_members):
// vidDIP_COUNTER (24B), vidDIP_ID (28B), vidDIP (60B), vidDIP_CLASS (48B), vidDIP_GROUP (48B),
// vidDIP_STAT (124B).

typedef struct vidDIP_COUNTER
{
    int dipCur;  /* 0x00 */
    int dipMax;  /* 0x04 */
    int polyCur; /* 0x08 */
    int polyMax; /* 0x0C */
    int dip;     /* 0x10 */
    int poly;    /* 0x14 */
} vidDIP_COUNTER;

typedef struct vidDIP_ID
{
    unsigned int    id;      /* 0x00 */
    vidDIP_COUNTER  counter; /* 0x04 */
} vidDIP_ID;

typedef struct vidDIP
{
    int                       dipIdCount; /* 0x00 */
    int                       dipIdMax;   /* 0x04 */
    int                       polyIdMax;  /* 0x08 */
    dsTSTRING<char>           name;       /* 0x0C */
    vidDIP_COUNTER            counter;    /* 0x10 */
    dsVECTOR<vidDIP_ID, 8>    dipIDList;  /* 0x28 */

    // 0x8266A850 -- reversed in vidDIP__vidDIP.c. Zero-inits the counters, adopts the shared
    // empty-string singleton for `name`, default-constructs dipIDList (with call-site cookie
    // {vid.h, 439}).
    vidDIP();

    // 0x82669808 -- reversed in vidDIP__dtor.c. Frees dipIDList's backing store and releases
    // `name`'s buffer reference.
    ~vidDIP();
} vidDIP;

typedef struct vidDIP_CLASS
{
    dsTSTRING<char>          name;    /* 0x00 */
    vidDIP_COUNTER            counter; /* 0x04 */
    dsVECTOR<vidDIP, 8>       dipList; /* 0x1C */

    // 0x8266A92C -- reversed in vidDIP_CLASS__vidDIP_CLASS.c. Mirrors vidDIP::vidDIP (call-site
    // cookie {vid.h, 454}).
    vidDIP_CLASS();

    // 0x8266A1D0 -- reversed in vidDIP_CLASS__dtor.c. Destroys dipList, releases `name`.
    ~vidDIP_CLASS();
} vidDIP_CLASS;

typedef struct vidDIP_GROUP
{
    dsTSTRING<char>              name;         /* 0x00 */
    vidDIP_COUNTER                counter;      /* 0x04 */
    dsVECTOR<vidDIP_CLASS, 8>     dipClassList; /* 0x1C */

    vidDIP_GROUP(); // boundary (outside this re-source)
    ~vidDIP_GROUP(); // boundary (outside this re-source)
} vidDIP_GROUP;

typedef struct vidDIP_STAT
{
    osLOCK                       lock;         /* 0x00 */
    dsVECTOR<vidDIP_GROUP, 8>    dipGroupList; /* 0x34 (52) */
    vidDIP_COUNTER                counter;      /* 0x48 (72) */
    vidDIP_GROUP                 *curGroup;     /* 0x60 (96) */
    vidDIP_CLASS                 *curClass;     /* 0x64 (100) */
    vidDIP                       *curDip;       /* 0x68 (104) */
    vidDIP_ID                    *curDipID;     /* 0x6C (108) */
    osTIMER                      timer;         /* 0x70 (112) */
    int                           isActive;      /* 0x78 (120) */

    // 0x8266D114 -- reversed in vidDIP_STAT__SetCurDip.c. Selects (creating on first use) the
    // current class/dip/dip-id records matching `nameClass`/`nameDip`/`dipID` under the already-
    // selected curGroup, guarded by `lock`. No-op when inactive or no group is selected.
    // Mangled signature (?SetCurDip@vidDIP_STAT@@QAAXPBD0K@Z): (const char*, const char*, unsigned long).
    void SetCurDip(const char *nameClass, const char *nameDip, unsigned long dipID);

    // 0x8266D3BC-adjacent -- select the active stat group by name. boundary (outside this re-source).
    void SetCurGroup(const char *nameGroup);

    // boundary (outside this re-source).
    void Activate(int active);
    void FinishFrame();
} vidDIP_STAT;
