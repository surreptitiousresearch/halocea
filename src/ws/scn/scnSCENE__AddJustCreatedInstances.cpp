#include "scnSCENE.h"
#include "../ds/dsCMP.h"
#include "../ds/ds_assert_boundary.h"

// ?GetPhysicsThreadNMB@@YAHXZ / ?GetMainThreadNMB@@YAHXZ are unrelated helpers; the thread guard
// here reads a plain file-scope int global and osGetCurThreadProcessor().
extern "C" int threadNmb;                                  // boundary — Scene.cpp file-scope global
extern "C" int osGetCurThreadProcessor();               // boundary

// ?AddJustCreatedInstances@scnSCENE@@QAAXH@Z  (0x82540BD8) — insert `id` into the sorted
// `instIndJustCreated` set (INS_DUP_IGNORE: an existing duplicate id is left alone). Also asserts
// this method is only ever called from the thread that first touched the scene (`threadNmb` is
// cached lazily on first call).
//
// NOTE: the decompiler fabricated 13 extra trailing int parameters (a3..a14) on this function.
// Disasm (0x82540bd8-0x82540c88) confirms only r3 (this) and r4 (id, stored to arg_1C and never
// otherwise read) are used — the real prototype takes a single int. The phantom params were
// dropped here and at both call sites (AllocInst, and AllocInst's caller CreateInst) after
// verifying against disasm.
void scnSCENE::AddJustCreatedInstances(int id)
{
    if (threadNmb == -1)
    {
        threadNmb = osGetCurThreadProcessor();
    }
    else if (!IGNORE_STRONG_ASSERT && threadNmb != osGetCurThreadProcessor())
    {
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "threadNmb == osGetCurThreadProcessor()",
            "D:\\Projects\\code\\common\\src.sys\\objects\\Scene.cpp",
            853,
            empty_string);
    }

    dsCMP cmp;
    this->instIndJustCreated.InsertSorted(id, cmp, dsVECTOR<int, 8>::INS_DUP_IGNORE);
}
