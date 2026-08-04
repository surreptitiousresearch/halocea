#pragma once
/* device_definition — the shared prefix of every device-derived tag body (machine, control,
   light_fixture): the object payload followed by the device payload. */

#include "_object_definition.h"
#include "_device_definition.h"

typedef struct device_definition
{
    _object_definition object;  /* 0x000 (380 bytes) */
    _device_definition device;  /* 0x17C (276 bytes) */
} device_definition;            /* 656 bytes */
