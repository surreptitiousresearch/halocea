// gsSR_MANAGER::CheckAddExecJobs @ 0x82D250B8
// Trivial: reset the per-frame collision-query counters.

#include "../../headers/ws/gs/gsSR_MANAGER.h"

void gsSR_MANAGER::CheckAddExecJobs()
{
    queryAddNmb = 0;
    queryExecuteNmb = 0;
}
