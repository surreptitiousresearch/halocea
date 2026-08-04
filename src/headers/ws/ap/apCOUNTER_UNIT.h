#pragma once
#include "../ds/dsTSTRING.h"
#include "apSTATE_T.h"
// ws-engine ap (platform): one named runtime performance counter / gauge.
// DB-verified layout (types_members apCOUNTER_UNIT) — size 48.

struct apPROFILER; // boundary — owning profiler (ap subsystem), only a pointer is stored

typedef struct apCOUNTER_UNIT {
    dsTSTRING<char>        name;            // 0x00 counter name
    apSTATE_T<unsigned char> state;         // 0x04 enabled/visibility state
    unsigned char _pad0[3]; /* db-verified padding */
    unsigned int           color;           // 0x08 graph color
    float                  graphScale;      // 0x0C
    float                  threshold;       // 0x10
    apPROFILER            *myProfiler;      // 0x14 owning profiler
    float                  value;           // 0x18 current sample
    float                  valueAverage;    // 0x1C
    float                  valueAverageOut; // 0x20
    float                  valuePeak;       // 0x24
    float                  valuePeakOut;    // 0x28
    int                    numValueChanged; // 0x2C

    // 0x82715988 — construct a counter with the given (possibly empty) name: default
    // state = 1 (enabled), zeroed color/profiler/scale-1/threshold/value/averages/peaks.
    apCOUNTER_UNIT(const char *name);
    apCOUNTER_UNIT() {}
} apCOUNTER_UNIT;
