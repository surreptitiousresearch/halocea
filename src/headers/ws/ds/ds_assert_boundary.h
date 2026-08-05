#pragma once
// Shared boundary declarations for the Saber STRONG_ASSERT debug machinery used by
// ds container methods. These are not decompiled here.

// ?IGNORE_STRONG_ASSERT@@3HA — global that disables the strong-assert crash path.
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */

// Empty context/message string global at 0x8200155A, passed as the final Crash argument.
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// STRONG_ASSERT_DUMMY / STRONG_ASSERT2_HELPER live in their canonical dbg headers (shared to avoid
// C2011 redefinition when multiple *_boundary.h headers are co-included).
#include "../dbg/STRONG_ASSERT_DUMMY.h"
#include "../dbg/STRONG_ASSERT2_HELPER.h"
