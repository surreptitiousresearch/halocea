#pragma once
#include "jbmJOB_EX.h"
// ws-engine jbm: a job whose sole purpose is to block (as a job-queue barrier) until another
// specific job has finished, letting a dependent stage be queued without an explicit
// WaitThreadsDone stall on every thread.
// DB-verified layout (types_members jbmJOB_EX_WAIT_JOB): jbmJOB_EX@0 (20B, base),
// pJobToWait@20 (jbmJOB_EX*) — size 24.

struct jbmJOB_EX_WAIT_JOB : jbmJOB_EX {
    jbmJOB_EX *pJobToWait; // 0x14 the job this barrier waits on
};
