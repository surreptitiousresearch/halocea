#pragma once
/* vehicle_datum — a live vehicle object datum: a 4-byte definition index, the shared object payload
 * (_object_datum), the unit payload (_unit_datum, vehicles are a kind of unit), and the
 * vehicle-specific payload (_vehicle_datum). Composite layout DB-verified (definition_index @0,
 * _object_datum object @4, _unit_datum unit @500, _vehicle_datum vehicle @1228; 1472 total).
 * The vehicle-level field block lives in _vehicle_datum.h. */

#include "_object_datum.h"
#include "_unit_datum.h"
#include "_vehicle_datum.h"

typedef struct vehicle_datum
{
    int            definition_index; /* 0x000 */
    _object_datum  object;           /* 0x004 (4) */
    _unit_datum    unit;             /* 0x1F4 (500) */
    _vehicle_datum vehicle;          /* 0x4CC (1228) */
} vehicle_datum;                     /* 1472 bytes */
