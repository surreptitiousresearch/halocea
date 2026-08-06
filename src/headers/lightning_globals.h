#pragma once
/* DB-verified: anonymous PDB struct $039D7A94EA641C1B0629B74F699EBAA2 (types.size = 4), sole
 * member lightning_data:data_array * at +0x00, from types_members. (Previously annotated
 * "no DB/PDB type": the type is keyed by the address of lightning_globals @ 0x844A7488 via
 * applied_types, not by name.) */
#include "data_array.h"

typedef struct lightning_globals_type
{
    data_array *lightning_data; /* 0x00 */
} lightning_globals_type;

#ifdef __cplusplus
extern "C" {
#endif

extern lightning_globals_type lightning_globals;

#ifdef __cplusplus
}
#endif
