#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Save @ 0x823BF380
// No serializer for this type: resolve the value pointer (side-effect-free), strong-assert("0")
// and return failure (0). fioFILE is a boundary.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Save(
        const unsigned int *storage, fioFILE *pFile) const
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::GetPtr(storage);
    if (!IGNORE_STRONG_ASSERT)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 186, empty_string);
    return 0;
}
