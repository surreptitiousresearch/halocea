#pragma once
#include <stdint.h>
/* hs_global_external — an engine-defined (non-scenario) HaloScript global descriptor. Only the leading type
 * field is referenced by the script subsystem; `name` and `address` follow (used by the name lookup). */

typedef struct hs_global_external
{
    const char    *name;          /* 0x00 */
    int16_t        type;          /* 0x04 */
    unsigned char _pad0[2]; /* db-verified padding */
    void          *pointer;       /* 0x08 — backing engine value (null => use the type default) */
    unsigned char  console_flags; /* 0x0C */
} hs_global_external;

extern hs_global_external *hs_external_globals[];
extern int16_t hs_external_global_count;
