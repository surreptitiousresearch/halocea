#pragma once
#include "propBASE.h"
// ws-engine prop subsystem: entity-facing prop base — the common ancestor of the concrete
// per-feature prop components (propINST_CONSTRUCTOR, etc).
// DB-verified layout (types_members propENT): a single anonymous propBASE member at offset 0,
// no additional fields — size 40, identical to propBASE. Modeled as public inheritance per the
// project's "anonymous leading struct member at offset 0" convention.

struct propENT : propBASE {
};
