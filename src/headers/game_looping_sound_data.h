#pragma once
/* game_looping_sound_data — DB-verified: applied_types @0x846DD0E0 is
 * `data_array *game_looping_sound_data;` (a datum pool, iterated with
 * DATA_ARRAY_ELEMENT). The prior reconstruction typed it as a standalone
 * game_looping_sound_data_t{valid}, which forced consumers to cast to
 * data_array* and broke direct ->data/->size access. Alias the reconstruction
 * name to data_array so all callers (cast and direct) agree with the DB. */

#include "data_array.h"

typedef data_array game_looping_sound_data_t;

#ifdef __cplusplus
extern "C" {
#endif

extern data_array *game_looping_sound_data;

#ifdef __cplusplus
}
#endif
