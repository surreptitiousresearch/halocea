#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include <cstring>

// ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::CopyObj() @ 0x823BC040
// Overwrite the already-boxed destination value from a raw object (the dest cell holds a pointer
// to the 0x44-byte boxed value).
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::CopyObj(const haloPLAYER_ACTIONS_ENV *obj, void **dest)
{
    memcpy(*dest, obj, 0x44u); // exact byte count from the binary (sizeof haloPLAYER_ACTIONS_ENV)
}
