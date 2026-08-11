#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

// ds_data::dsDATA_TYPE_STREAM<haloPLAYER_ACTIONS_ENV>::Save() @ 0x823BBC30
// Disk serialization is unsupported for this value type: the template's default Save asserts
// ("0") and returns 0 (ds/ds_data_type_impl.hpp:186).
template<>
int ds_data::dsDATA_TYPE_STREAM<haloPLAYER_ACTIONS_ENV>::Save(const haloPLAYER_ACTIONS_ENV &data, fioFILE *pFile)
{
    if (!IGNORE_STRONG_ASSERT)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 186, "");
    return 0;
}
