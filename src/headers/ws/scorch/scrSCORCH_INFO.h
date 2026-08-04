#pragma once
// ws-engine scorch: global scorch statistics counters. These are referenced in the DB as static
// class members `scrSCORCH_INFO::<name>` (each a plain int global, reset by RemoveAll). The class
// carries no instance members in the DB, so it is modelled as a static-member holder.

struct scrSCORCH_INFO {
    static int nScorchStat;
    static int nScorchDyn;
    static int nFacesDyn;
    static int nFacesStat;
    static int nVertDyn;
    static int nVertStat;
    static int nBatchStat;
    static int nBatchDyn;
};
