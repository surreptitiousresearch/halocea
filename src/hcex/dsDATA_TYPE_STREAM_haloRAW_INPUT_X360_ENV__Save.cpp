#include "../headers/ws/ds/dsDATA_TYPE_STREAM.h"
#include "../headers/ws/ds/ds_assert_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STREAM<haloRAW_INPUT_X360_ENV>::Save @ 0x823BCC10
// This type opts out of disk streaming: the stub trips a strong-assert ("0" is unconditionally
// false) at ds_data_type_impl.hpp:186 and returns 0.
template<>
int ds_data::dsDATA_TYPE_STREAM<haloRAW_INPUT_X360_ENV>::Save(
        const haloRAW_INPUT_X360_ENV &data, fioFILE *pFile)
{
    if (!IGNORE_STRONG_ASSERT)
        STRONG_ASSERT_DUMMY::Crash(
            nullptr, "0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 186,
            (unsigned char)dsStrongAssertMessage[0]);
    return 0;
}
