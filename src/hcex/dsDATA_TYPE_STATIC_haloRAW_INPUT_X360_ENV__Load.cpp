#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

extern int IGNORE_STRONG_ASSERT;   // ?IGNORE_STRONG_ASSERT@@3HA
extern const char empty_string;   // shared single-NUL empty-string constant

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Load @ 0x823BDF60
// No disk deserializer for this type: strong-assert("0") and return failure (0). fioFILE is a boundary.
template<>
int ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Load(fioFILE *pFile, haloRAW_INPUT_X360_ENV &data)
{
    if (!IGNORE_STRONG_ASSERT)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 187, empty_string);
    return 0;
}
