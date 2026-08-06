// scrSCORCH_MANAGER::Add @0x82739078
//
// Register one new scorch/decal described by `iniIn` (+ its resolved descriptor `scrDescIn`):
//   0. Profiled under the "process_frame/scrSCORCH_MANAGER::Add" timer counter.
//   1. Optional debug filter: when the dbg_dontAddScorch console var is set and names a non-empty
//      class that differs from the incoming class, the add is allowed; otherwise it is dropped.
//      (Reproduced faithfully from the disassembly's inline class-name compare.)
//   2. Bump a reference on the incoming descriptor and validate the (ini, desc) pair via
//      IsValidInput; bail out if invalid.
//   3. Take a working copy of the ini, stamp its timeAdded from the level timer, then under the
//      global MT lock: collect the affected polygons (which may substitute the descriptor), add the
//      resulting static and dynamic scorch marks, and re-prepare the render batches.
//
// DEVIATIONS:
//  - The compiler inlined the apCOUNTER_TIME_CALL_WRP RAII guard and the one-shot magic-static
//    counter ctor; reconstructed here using the RAII wrapper directly (see SwapBuffers.cpp).
//  - The decompiler modelled CollectPolygons' by-value dsSMART_PTR return via an sret slot and
//    duplicated `this`/arg registers; reconstructed with the DB-verified signature.
//  - The working ini copy's trailing manual nameClass refcount-decrement/free is the scrSCORCH_INI
//    dtor, expressed here as ordinary RAII scope exit.
#include "../../headers/ws/scorch/scrSCORCH_MANAGER.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"
#include "../../headers/ws/gs/gsLVL_TIMER.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// ---- boundary externs (deeper than the 2-level descent budget) -------------------------------
struct osLOCK;
extern "C" osLOCK g_syncMT;
void osLOCK__Lock(osLOCK *self, void *, int);   // osLOCK::Lock
void osLOCK_Unlock(osLOCK *self, void *, int); // osLOCK::Unlock
extern "C" long osLockedIncrement(volatile int *p);

// Debug console variables (dbgVAR<T>) gating scorch creation. boundary — the .value field is the
// live typed value.
struct dbgVAR_bool  { int   value; };
struct dbgVAR_class { dsTSTRING<char> value; };
extern "C" dbgVAR_bool  dbg_dontAddScorch;
extern "C" dbgVAR_class dbg_dontAddScorchClass;

void scrSCORCH_MANAGER::Add(
    const scrSCORCH_INI *iniIn,
    dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > *scrDescIn)
{
    static apCOUNTER_TIME_CALL addCounter("process_frame/scrSCORCH_MANAGER::Add");
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> timerScope(&addCounter);

    // (1) Debug class filter: proceed only when the filter names a non-empty class distinct from
    // the incoming one. An unset dbg_dontAddScorch var proceeds unconditionally.
    if (dbg_dontAddScorch.value)
    {
        bool filterClassNonEmpty = dbg_dontAddScorchClass.value.pBuffer->str[0] != '\0';
        if (!filterClassNonEmpty || dbg_dontAddScorchClass.value == iniIn->nameClass)
            return; // filtered out
    }

    // (2) Adopt a reference on the incoming descriptor, then validate the pair.
    dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > desc;
    scrSCORCH_DESC *pointee = scrDescIn->pointee;
    if (!pointee || pointee == (scrSCORCH_DESC *)-4)
    {
        desc.pointee = nullptr;
    }
    else
    {
        osLockedIncrement((volatile int *)&((dsREF_COUNT *)pointee)->refCount);
        desc.pointee = pointee;
    }

    if (IsValidInput(*iniIn, desc))
    {
        // (3) Working copy of the ini, timestamped from the level timer.
        scrSCORCH_INI ini(*iniIn);
        ini.timeAdded = gsLvlTimer->timeLife;

        osLOCK__Lock(&g_syncMT, nullptr, 0);
        dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > usedDesc =
            CollectPolygons(ini, *scrDescIn, 0);
        AddStatScorchmarks(ini, usedDesc, true);
        AddDynScorchmarks(ini, usedDesc);
        PrepareRender();
        desc.DeletePointee();
        osLOCK_Unlock(&g_syncMT, nullptr, 0);
        // `ini` (and its nameClass) released here by RAII.
    }

    // Release the caller's descriptor reference (matches the tail DeletePointee in the disasm).
    scrDescIn->DeletePointee();
}
