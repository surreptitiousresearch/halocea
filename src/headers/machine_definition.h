#pragma once
/* machine_definition — composite 'mach' tag definition: shared object level, device level, then the
 * machine-level payload. Composite layout DB-verified (object @0, device @380, machine @656;
 * 804 bytes total, 2026-07-14). */

#include "_object_definition.h"
#include "_device_definition.h"
#include "_machine_definition.h"

typedef struct machine_definition
{
    _object_definition  object;   /* 0x000 */
    _device_definition  device;   /* 0x17C (380) */
    _machine_definition machine;  /* 0x290 (656) */
} machine_definition;             /* 804 bytes */
