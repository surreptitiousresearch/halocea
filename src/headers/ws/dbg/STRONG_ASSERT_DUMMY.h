#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine debug: the Saber "strong assertion" crash reporter. STRONG_ASSERT_DUMMY is a
// zero-real-state helper (a single int slot the crash path stores 0 into to force a null
// dereference in non-build mode). DB layout (types_members STRONG_ASSERT_DUMMY): dummy@0.
//
// This is the canonical home of the body reversed in STRONG_ASSERT_DUMMY__Crash.cpp; it is the
// target of the hcex bridge's widely-used flattened extern STRONG_ASSERT_DUMMY_Crash.

struct STRONG_ASSERT_DUMMY {
    int dummy; // 0x00

    // 0x825202A8 (?Crash@STRONG_ASSERT_DUMMY@@QAAXPBD0H0@Z) — format the failed condition + source
    // location + caller info, print/log/msgbox it, break into the debugger, and (release builds)
    // deliberately fault. Runs at most once (guarded by the bHappend global).
    void Crash(const char *condition, const char *file, int line, const char *info);

    // Static call forms used by the *_boundary.h reversings (differ in parameter-type-list from the
    // member Crash above, so they coexist as overloads). Same 0x825202A8 target; layout-neutral.
    static void Crash(STRONG_ASSERT_DUMMY *unused, const char *condition, const char *file, int line, const char *info);
    static void Crash(void *ctx, const char *expr, const char *file, int line, unsigned char msgCtx);

    // ?m_bBuildMode@STRONG_ASSERT_DUMMY@@2_NA @ 0x841DB14C — when set (tool/build mode), suppress
    // the message box, the debug break and the forced fault.
    static bool m_bBuildMode;
};
