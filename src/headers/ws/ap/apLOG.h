#pragma once
#include "../os/osFILE_HANDLE_DUMMY.h"
// ws-engine ap (ap_log.cpp): one named log-file slot. The engine keeps a fixed table of these
// (_apLogList) keyed by absolute path; apForceLogV opens/appends/closes them by name.
// DB-verified layout (types_members apLOG), size 264: stream@0, name@4 (char[260]).

typedef struct apLOG {
    osFILE_HANDLE_DUMMY *stream; // 0x00 open OS file handle (null = vacant slot)
    char                 name[260]; // 0x04 absolute path (empty string = vacant slot)
} apLOG;

// ?_apLogList / the fixed slot table. The scan bound in apForceLogV is the address of the
// adjacent global `gs_prefix`; the code treats slot index 250 as the "not found" sentinel.
extern "C" apLOG _apLogList[250];
