#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include <cstring>

// ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Copy() @ 0x823BBFE8
// Overwrite the already-boxed destination value from the boxed source (both cells hold pointers to
// a 0x44-byte haloPLAYER_ACTIONS_ENV).
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Copy(const void **src, void **dest)
{
    memcpy(*dest, *src, 0x44u); // exact byte count from the binary (sizeof haloPLAYER_ACTIONS_ENV)
}
