#pragma once
#include "propBASE_DESC.h"
// ws-engine prop: entity-flavoured property descriptor (adds no new data of its own over
// propBASE_DESC in this DB; parallels propENT).
// DB-verified layout (types_members propENT_DESC): <propBASE_DESC base>@0 — size 52.

struct propENT_DESC : propBASE_DESC {
};
