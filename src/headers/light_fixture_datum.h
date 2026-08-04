#pragma once

/* light_fixture_datum — object-pool datum for a scenario light-fixture (a device object).
 * Composite layout DB-verified (types_members light_fixture_datum): a generic _object_datum
 * followed by the shared _device_datum extension and the light-fixture-specific tail. */

#include "_object_datum.h"
#include "device_datum.h"
#include "_light_fixture_datum.h"

typedef struct light_fixture_datum
{
    int                  definition_index;   /* 0x000 */
    _object_datum        object;             /* 0x004 */
    _device_datum        device;              /* 0x1F4 (500) */
    _light_fixture_datum light_fixture;      /* 0x214 (532) */
} light_fixture_datum;
