#pragma once
// Canonical apDEFRAG_BUF definition lives in the flat header src/headers/apDEFRAG_BUF.h, which
// every TU picks up via `#include "apDEFRAG_BUF.h"` (src\headers is first on the include path).
// Forward to it so a TU reaching this organized-tree path does not get a duplicate typedef (C2011).
// Identical DB layout; this mirrors the sibling ws/ap/apDEFRAG_CHUNK.h forwarder.
#include "../../apDEFRAG_BUF.h"
