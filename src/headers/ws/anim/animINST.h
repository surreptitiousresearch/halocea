#pragma once
// Canonical animINST definition lives in the flat header src/headers/animINST.h, which is the
// one every TU picks up via `#include "animINST.h"` (src\headers is first on the include path).
// This organized-tree copy forwards to it so a TU that reaches this path (e.g. scn headers using
// "../anim/animINST.h") gets the same single definition instead of a duplicate typedef (C2011).
#include "../../animINST.h"
