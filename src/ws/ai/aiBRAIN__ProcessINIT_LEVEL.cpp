#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

// ai planner singleton (?aiPlanner@@3PAVaiPLANNER@@A). Declared locally to avoid pulling the
// aiEXIFACE boundary header (which re-defines aiPLANNER as a subset). boundary data.
extern aiPLANNER *aiPlanner;

// Strong-assert plumbing (mirrors the cseries STRONG_ASSERT macro). Declared locally for the same
// reason. boundary.
extern int IGNORE_STRONG_ASSERT;
extern unsigned char STRONG_ASSERT_INFO_STUB; // byte_8200155A
// STRONG_ASSERT_DUMMY::Crash comes from the canonical dbg header (was previously a local redef).

// aiBRAIN::ProcessINIT_LEVEL @ 0x8317AA18
// ?ProcessINIT_LEVEL@aiBRAIN@@UAAHXZ
//
// Per-level brain init: run the iaIACTOR base init, register the unit with the planner, init the
// body, refresh the body coordinate system, and seed the initial mind state. Returns the base
// init result.
int aiBRAIN::ProcessINIT_LEVEL()
{
    int baseResult = this->iaIACTOR::ProcessINIT_LEVEL();

    if (!IGNORE_STRONG_ASSERT && !aiPlanner)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "aiPlanner",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_init.cpp", 888,
            STRONG_ASSERT_INFO_STUB);

    aiPlanner->RegisterUnit(this);
    this->body.pointee->ProcessInitLevel();
    this->UpdateBodyCS();
    this->mind.SetupInitial();

    return baseResult;
}
