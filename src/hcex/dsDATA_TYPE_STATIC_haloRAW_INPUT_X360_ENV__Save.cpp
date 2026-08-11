// FUNCTION_INDEX entry: ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Save @0x823BDF00 (?Save@?$dsDATA_TYPE_STATIC@UhaloRAW_INPUT_X360_ENV@@@ds_data@@SAHABUhaloRAW_INPUT_X360_ENV@@PAVfioFILE@@@Z)
#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Save @ 0x823BDF00
// No disk serializer for this type: strong-assert("0") and return failure (0). fioFILE is a boundary.
template<>
int ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Save(const haloRAW_INPUT_X360_ENV &data, fioFILE *pFile)
{
    if (!IGNORE_STRONG_ASSERT)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 186, empty_string);
    return 0;
}
