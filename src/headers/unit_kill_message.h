#pragma once
/* unit_kill_message — the legacy local name unit_kill_from_network.c uses for the "unit kill"
 * message-delta decode target. It is the SAME type as unit_kill_network_data (DB: types_members
 * unit_kill_network_data, 12 members, 32 bytes) — this header used to restate the whole body,
 * which is one `error: redefinition` the moment both headers land in a translation unit and is
 * why the type was invisible to header_layout's probe. Now a pure alias of the canonical
 * definition; the per-field provenance comments this copy carried were merged into it. */

#include "unit_kill_network_data.h"

typedef unit_kill_network_data unit_kill_message;
