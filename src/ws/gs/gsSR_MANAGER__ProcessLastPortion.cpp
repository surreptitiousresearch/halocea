// gsSR_MANAGER::ProcessLastPortion @ 0x82D26A00
// Start the PPU and SPU int-managers' last-portion jobs (the one indexed by lastJobNmb), then
// run all delayed jobs on the srMANAGER base.
//
// Deviation from decompiler: the PPC pseudocode mislabels the field accesses using the phantom
// boundary `srMANAGER` model (`&this->srMANAGER + 4`, `this->pScene`). The disassembly
// (0x82D26A14: r3 = this+0x20+4; 0x82D26A2C: r11 = this+0x38) shows the PPU manager at this+0x20
// (mngPPU) and the SPU manager at this+0x38 (mngSPU); modeled directly.

#include "../../headers/ws/gs/gsSR_MANAGER.h"

void gsSR_MANAGER::ProcessLastPortion()
{
    // operator[] yields the element (a per-processor delayed-job-manager pointer); the manager
    // records derive from jbmJOB, so they pass to gsStartJobSrMng(jbmJOB*).
    gsStartJobSrMng(reinterpret_cast<jbmJOB *>(mngPPU.jobs[mngPPU.lastJobNmb]));
    gsStartJobSrMng(reinterpret_cast<jbmJOB *>(mngSPU.jobs[mngSPU.lastJobNmb]));
    // RunAllDelayedJobs is dispatched on the srMANAGER base (disasm passes this+0).
    reinterpret_cast<srMANAGER *>(this)->RunAllDelayedJobs();
}
