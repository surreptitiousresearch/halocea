#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include <cstring>

// ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Copy() @ 0x823BBFE8
// Overwrite the already-boxed destination value from the boxed source (both cells hold pointers to
// a 0x44-byte haloPLAYER_ACTIONS_ENV).
// DEVIATION: the cells were spelled `const void **`/`void **` here; the DB mangling is
// `SAXPBKPAK@Z` (`const unsigned long *`, `unsigned long *`) and the canonical
// ws/ds/dsDATA_TYPE_CONSTRUCT.h now declares the one 4-byte-cell spelling.
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Copy(const unsigned int *src, unsigned int *dest)
{
    // exact byte count from the binary (sizeof haloPLAYER_ACTIONS_ENV)
    memcpy(reinterpret_cast<void *>(*dest), reinterpret_cast<const void *>(*src), 0x44u);
}
