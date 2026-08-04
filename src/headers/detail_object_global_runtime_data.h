#pragma once
/* detail_object_global_runtime_data — per-player runtime state for the structure detail-object renderer.
 * Layout verbatim from the database. */

#include "real_vector4d.h"
#include "detail_object_runtime_data.h"

struct detail_object_global_runtime_data
{
    detail_object_runtime_data local_player_data[2];        /* 0x0000 */
    real_vector4d              default_z_reference_vector;  /* 0xA420 */
}; /* 42032 bytes */

extern struct detail_object_global_runtime_data *detail_object_global_runtime_data;
