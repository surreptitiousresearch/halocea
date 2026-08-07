#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include <cstring>

// ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::CopyObj() @ 0x823BC040
// Overwrite the already-boxed destination value from a raw object (the dest cell holds a pointer
// to the 0x44-byte boxed value).
// DEVIATION: the cell was spelled `void **` here; the DB mangling is
// `SAXPBUhaloPLAYER_ACTIONS_ENV@@PAK@Z` (`unsigned long *`) and the canonical
// ws/ds/dsDATA_TYPE_CONSTRUCT.h now declares the one 4-byte-cell spelling.
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::CopyObj(const haloPLAYER_ACTIONS_ENV *obj,
                                                                     unsigned int *dest)
{
    // exact byte count from the binary (sizeof haloPLAYER_ACTIONS_ENV)
    memcpy(reinterpret_cast<void *>(*dest), obj, 0x44u);
}
