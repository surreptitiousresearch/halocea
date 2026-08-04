#pragma once

#include "ws/ds/dsARRAY_global.h"    // global dsARRAY<T> (labels); NOT ds::ARRAY from dsARRAY.h

/* Pointer-only dependency (boundary). */
typedef struct vidDRIVER_INTERFACE vidDRIVER_INTERFACE;

/* GPU page allocator used by the D3D driver's command-buffer path.
   DB-verified layout (types_members vidPAGE_MEMORY_ALLOCATOR). */
typedef struct vidPAGE_MEMORY_ALLOCATOR
{
    int                   memAlignment;                     /* 0x00 */
    int                   memSize;                          /* 0x04 */
    int                   memPagesCount;                    /* 0x08 */
    unsigned int          nextFreePage;                     /* 0x0C */
    unsigned char        *allocatedMemory;                  /* 0x10 */
    vidDRIVER_INTERFACE  *drv;                              /* 0x14 */
    dsARRAY<unsigned long> labels;                          /* 0x18 */
    int                   statFrameMemUsage[6];             /* 0x28 */
    int                   statFrameMemUsageSPU[6];          /* 0x40 */
    int                   statFrameChunksSkipped;           /* 0x58 */
    int                   statNonfreePagesFromPrevFrame;    /* 0x5C */
    int                   statNonfreePagesGPUFromPrevFrame; /* 0x60 */
} vidPAGE_MEMORY_ALLOCATOR;
