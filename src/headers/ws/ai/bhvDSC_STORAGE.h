#pragma once
#include "../dsc/dscDESC_FAMILY.h"

// ws-engine ai08 — the behavior-descriptor family: the dscDESC_FAMILY specialisation that owns the
// registered AI behavior brands (bvd* descriptors). DB-verified layout (types_members
// bhvDSC_STORAGE): a single dscDESC_FAMILY base @0 — size 140 (0x8C).

struct bhvDSC_STORAGE : dscDESC_FAMILY {
};
