#include <stdint.h>
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"
#include "../headers/ws/ds/dsDATA.h"

// 0x823BD3A8 — deserialize in the same field order as Pack.
// DEVIATION: the disassembly writes the `buttons` read via `RawRead(stream, this, 0x20u, 0)`
// (offset 0 of the object, i.e. the inherited haloRAW_INPUT_X360::buttons field, since
// haloRAW_INPUT_X360_ENV's base sits at offset 0) rather than `&this->buttons` explicitly --
// reproduced below as the equivalent named-field access.
void haloRAW_INPUT_X360_ENV::Unpack(ds::BIT_STREAM &stream, const dsDATA &userCtx)
{
    stream.RawRead(&msgId, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&msgId), 4, 4);

    stream.RawRead(&tick, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&tick), 4, 4);

    stream.RawRead(&crc, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&crc), 4, 4);

    stream.RawRead(&buttons, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&buttons), 4, 4);

    stream.RawRead(&sThumbLX, 0x10u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&sThumbLX), 2, 2);

    stream.RawRead(&sThumbLY, 0x10u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&sThumbLY), 2, 2);

    stream.RawRead(&sThumbRX, 0x10u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&sThumbRX), 2, 2);

    stream.RawRead(&sThumbRY, 0x10u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&sThumbRY), 2, 2);
}
