#pragma once
#include "propENT_DESC.h"
// ws-engine prop: descriptor for a network-syncable property. Adds no data of its own over
// propENT_DESC in this DB — it exists to carry the syncable vtable slots.
// DB-verified layout (types_members propSYNCABLE_DESC): <propENT_DESC base>@0 — size 52.

struct propSYNCABLE_DESC : propENT_DESC {
};
