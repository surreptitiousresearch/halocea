#include "../../headers/ws/jbm/jbmTHREAD.h"

// boundary — os subsystem (matches convention in src/ws/scn/scnSCENE__AddJustCreatedInstances.cpp)
extern "C" int  osGetCurThreadProcessor();
extern "C" void osResetSignal(osHANDLE_DUMMY *signal); // boundary — os subsystem
extern "C" void osSetSignal(osHANDLE_DUMMY *signal);   // boundary — os subsystem

// 0x826EC800 jbmTHREAD::AddJob(jbmJOB*) — queue pJob onto this thread, or run it inline
// if the caller is already on this thread's own processor.
// DEVIATION: the decompiler reported 14 parameters (a3..a13, a14) beyond (this, pJob).
// Disasm (826ec800-826ec8a0) shows only r3 (this) and r4 (pJob) are ever read inside this
// function -- the extra params are dead caller-side registers the decompiler's __fastcall
// arg-collector mis-attributed (they are the spurious v9..v20 locals seen at the
// jbmMANAGER::AddJob call site). The mangled name `?AddJob@jbmTHREAD@@QAAXPAVjbmJOB@@@Z`
// also decodes to a single jbmJOB* parameter, confirming the true signature.
void jbmTHREAD::AddJob(jbmJOB *pJob)
{
    pJob->OnAdd(procID);

    if (procID == osGetCurThreadProcessor())
    {
        ExecuteJob(pJob);
    }
    else
    {
        lock.Lock("D:\\Projects\\code\\common\\src.sys\\job_mng\\job_mng.cpp", 251);
        jobDeque.PushBack(pJob);
        osResetSignal(threadDone);
        osSetSignal(startThread);
        lock.Unlock("D:\\Projects\\code\\common\\src.sys\\job_mng\\job_mng.cpp", 256);
    }
}
