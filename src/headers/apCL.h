#pragma once
/* apCL — ws-engine source-location cookie ({file, line}), initialized at runtime. */

typedef struct apCL
{
    const char *file; /* 0x0 */
    int         line; /* 0x4 */
} apCL;
