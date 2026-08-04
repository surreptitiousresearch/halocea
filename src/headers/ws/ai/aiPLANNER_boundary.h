#pragma once
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
#include "../nav/navMANAGER.h"

// Shared boundary declarations for the a8_planner.cpp batch: globals and cross-subsystem
// free functions / methods the planner calls but that are reversed in their own subsystems.

struct aiBRAIN;
struct navSYS;

extern "C" void dlFree(void *ptr);

// ap logging (variadic ~AI~ channel). boundary
extern void _apLog(const char *fmt, ...);

// gs app-state / player access. boundary
extern int   gsAppState;
extern void *gsGetPlayer(int index);

// farm subsystem — vis-change unfarming flag lives at farmSYSTEM+212. Only that field is needed
// here; the rest of farmSYSTEM is a boundary. DB-verified offset (types_members farmSYSTEM).
struct farmSYSTEM {
    unsigned char _pad0[212];        // 0x000  boundary
    int           isUnfarmingAfterVisChange; // 0x0D4
};
extern farmSYSTEM *farmSys;

// navMANAGER is defined once in nav/navMANAGER.h (included above).
