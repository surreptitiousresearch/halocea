#include <stdint.h>
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include "../headers/ws/ds/dsDATA.h"

// 0x823BC88C — deserialize in the same field order as Pack.
void haloPLAYER_ACTIONS_ENV::Unpack(ds::BIT_STREAM &stream, const dsDATA &userCtx)
{
    u0.Unpack(stream, userCtx);
    u1.Unpack(stream, userCtx);

    stream.RawRead(&skip_cine, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&skip_cine), 4, 4);
}
