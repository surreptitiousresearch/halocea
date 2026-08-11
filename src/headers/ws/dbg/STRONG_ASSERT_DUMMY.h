#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine debug: the Saber "strong assertion" crash reporter. STRONG_ASSERT_DUMMY is a
// zero-real-state helper (a single int slot the crash path stores 0 into to force a null
// dereference in non-build mode). DB layout (types_members STRONG_ASSERT_DUMMY): dummy@0.
//
// This is the canonical home of the body reversed in STRONG_ASSERT_DUMMY__Crash.cpp, and the one
// declaration of Crash in the corpus — every caller includes this header and calls the member.
//
// EXACTLY ONE Crash symbol exists (?Crash@STRONG_ASSERT_DUMMY@@QAAXPBD0H0@Z, `Q` = public
// NON-static member, 5 registers). Every call site passes a LITERAL NULL this — `li r3, 0`
// immediately before the bl — and Crash stores 0 through it: the null `this` IS the deliberate
// fault mechanism, not a modelling artefact. Spell every call site
//     static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(condition, file, line, info);
// The two `static` overloads this type used to carry were link-level fabrications: they mangle to
// ?Crash@STRONG_ASSERT_DUMMY@@SAX...@Z, absent from the image. Both are now unreachable from any
// in-scope caller (see each one below) — the wrong spellings stop compiling.
// `STRONG_ASSERT_DUMMY()` is deleted for the same reason: a temp emits `addi r3, r1, off`, never
// the `li r3, 0` the binary shows. Neither guard emits any code.

struct STRONG_ASSERT_DUMMY {
    int dummy; // 0x00

    STRONG_ASSERT_DUMMY() = delete; // corpus guard, not an as-built claim: never constructed

    // 0x825202A8 (?Crash@STRONG_ASSERT_DUMMY@@QAAXPBD0H0@Z) — format the failed condition + source
    // location + caller info, print/log/msgbox it, break into the debugger, and (release builds)
    // deliberately fault. Runs at most once (guarded by the bHappend global).
    void Crash(const char *condition, const char *file, int line, const char *info);

    // FABRICATIONS, retained ONLY because src/ws/ai/ is permanently user-excluded from work of any
    // kind and its 59 call sites (44 files) still spell the call this way. Both tails are wrong:
    // the binary loads the ADDRESS of the shared "" literal into r7 (`addi r7, rN,
    // byte_8200155A@l  # info`), so `info` is a `const char *` and the real call takes 4 args.
    //
    // NEITHER IS REACHABLE FROM IN-SCOPE CODE, which is what keeps this ratchet real:
    //   - the byte form needs an `unsigned char` tail; every in-scope site passes a pointer.
    //   - the pointer form's `char *` tail is DELIBERATELY NON-CONST — it is not an ABI claim.
    //     It exists for the two sites in src/ws/ai/aiNAV_WP_WRAPPER_TPL__ChangeOrAdd.cpp that
    //     pass `&byte_8200155A` off a TU-local `extern char`. Every in-scope caller passes
    //     `empty_string` or a string literal (`const char *`), which will not bind.
    // Delete both, with those 59 sites, if src/ws/ai/ is ever re-authorized.
    static void Crash(void *ctx, const char *expr, const char *file, int line, unsigned char msgCtx);
    static void Crash(void *ctx, const char *expr, const char *file, int line, char *info);

    // ?m_bBuildMode@STRONG_ASSERT_DUMMY@@2_NA @ 0x841DB14C — when set (tool/build mode), suppress
    // the message box, the debug break and the forced fault.
    static bool m_bBuildMode;
};
