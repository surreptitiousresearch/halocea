#pragma once
// Canonical apDEFRAG_CHUNK definition lives in the flat header src/headers/apDEFRAG_CHUNK.h, which
// every TU picks up via `#include "apDEFRAG_CHUNK.h"` (src\headers is first on the include path).
// Forward to it so a TU reaching this organized-tree path does not get a duplicate typedef (C2011).
// Identical DB layout; the dtor slot is exposed under both names (dtr / dtr_apDEFRAG_CHUNK) there.
#include "../../apDEFRAG_CHUNK.h"
