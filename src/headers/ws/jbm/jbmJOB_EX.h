#pragma once
#include "jbmJOB.h"
// ws-engine jbm: jbmJOB subclass that adds cross-thread completion tracking (an OS handle the
// enqueuing thread can wait on) plus the id of the worker currently executing it.
// DB-verified layout (types_members jbmJOB_EX): jbmJOB@0 (12B, base), threadIDCur@12 (int),
// jobDone@16 (osHANDLE_DUMMY*) — size 20.

struct osHANDLE_DUMMY; // boundary — os subsystem handle wrapper

struct jbmJOB_EX : jbmJOB {
    int             threadIDCur; // 0x0C worker index currently running this job
    osHANDLE_DUMMY *jobDone;     // 0x10 signalled when the job completes
};
