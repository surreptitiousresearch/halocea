#pragma once
#include "../dsc/dscDESC.h"
#include "sslCLASS_REF.h"
// ws-engine ssl: descriptor base for any dscDESC that carries an SSL script class binding
// (propBASE_DESC and its derivatives sit on top of this).
// DB-verified layout (types_members sslDESC): <dscDESC base>@0 (12), isUnshared@12,
// sslClass@16 — size 20.

struct sslDESC : dscDESC {
    bool         isUnshared; // 0x0C
    unsigned char _pad0[3]; /* db-verified padding */
    sslCLASS_REF sslClass;   // 0x10
};
