#pragma once
#include "../ds/dsTYPE_ID.h"
// ws-engine ai08: descriptor for an aiPROP_FIREABLE (the parsed, shared fireable/health config).
// BOUNDARY — only the class's static runtime-type id is modeled here (the sole surface reached by
// aiBRAIN::GetHealthPercent, which probes propContainer.typeId against it). The full descriptor
// layout is the next frontier.
struct aiPROP_FIREABLE_DESC {
    // DB-verified static (?TYPE_ID@aiPROP_FIREABLE_DESC@@2VdsTYPE_ID@@A) — this class's runtime-type
    // bitmask, used to detect a fireable property object on a container.
    static dsTYPE_ID TYPE_ID;
};
