#include "../headers/ws/ds/dsDATA_TYPE_STREAM.h"
#include "../headers/ws/ds/ds_assert_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STREAM<haloRAW_INPUT_X360_ENV>::Load @ 0x823BCC70
// Mirror of Save: this type opts out of disk streaming; the stub trips a strong-assert at
// ds_data_type_impl.hpp:187 and returns 0.
template<>
int ds_data::dsDATA_TYPE_STREAM<haloRAW_INPUT_X360_ENV>::Load(
        fioFILE *pFile, haloRAW_INPUT_X360_ENV &data)
{
    if (!IGNORE_STRONG_ASSERT)
        STRONG_ASSERT_DUMMY::Crash(
            nullptr, "0",
            "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 187,
            (unsigned char)empty_string[0]);
    return 0;
}
