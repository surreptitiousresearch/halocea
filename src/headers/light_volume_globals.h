#pragma once
/* DB-verified: anonymous PDB struct $0C063A19751579468C6CF468ED4D2770 (types.size = 4), sole
 * member light_volume_data:data_array * at +0x00, from types_members. (Previously annotated
 * "no DB/PDB type": the type is keyed by the address of light_volume_globals @ 0x844A7540 via
 * applied_types, not by name.) */
#include "data_array.h"

typedef struct light_volume_globals_type
{
    data_array *light_volume_data; /* 0x00 */
} light_volume_globals_type;

#ifdef __cplusplus
extern "C" {
#endif

extern light_volume_globals_type light_volume_globals;

#ifdef __cplusplus
}
#endif
