#pragma once
#include "../dsc/dscDESC.h"
// ws-engine gs: descriptor for a gsOBJ_MODIFIER_BASE-derived modifier (parsed config template
// that OBJECT_POLICY/EventModifier looks up by name to instantiate/drive a live modifier).
// DB-verified layout (types_members gsOBJ_MODIFIER_BASE_DESC): <dscDESC base>@0 — size 12.

struct gsOBJ_MODIFIER_BASE_DESC : dscDESC {
};
