#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../dip/dipPOINT.h"
// ws-engine ai08: per-domain saved spawn-point file record (aiSPAWN_SYSTEM::domainFileInfo
// element). DB-verified layout (types_members aiSPAWN_FILE_INFO) — size 24 (0x18):
// nameDomain@0, pointsList@4.

struct aiSPAWN_FILE_INFO {
    dsTSTRING<char>       nameDomain; // 0x00 domain name the points belong to
    dsVECTOR<dipPOINT, 8> pointsList; // 0x04 saved spawn points
};
