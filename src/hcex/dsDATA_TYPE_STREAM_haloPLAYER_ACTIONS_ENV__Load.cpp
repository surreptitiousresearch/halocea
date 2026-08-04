#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

// ds_data::dsDATA_TYPE_STREAM<haloPLAYER_ACTIONS_ENV>::Load() @ 0x823BBC90
// Disk deserialization is unsupported for this value type: the template's default Load asserts
// ("0") and returns 0 (ds/ds_data_type_impl.hpp:187).
template<>
int ds_data::dsDATA_TYPE_STREAM<haloPLAYER_ACTIONS_ENV>::Load(fioFILE *pFile, haloPLAYER_ACTIONS_ENV &data)
{
    if (!IGNORE_STRONG_ASSERT)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 187, "");
    return 0;
}
