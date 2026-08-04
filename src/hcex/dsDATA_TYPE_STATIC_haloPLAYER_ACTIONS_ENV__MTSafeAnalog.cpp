#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsDATA_TYPE.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::MTSafeAnalog @ 0x823BD9A0
// Return the MT-safe type analog for this type (id 0).
template<>
const dsDATA_TYPE *ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::MTSafeAnalog()
{
    return ds_data::GetMTSafeTypeAnalog(0);
}
