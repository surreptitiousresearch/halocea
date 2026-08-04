#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"
#include "../headers/ws/ds/dsDATA.h"

// 0x823BD180 — serialize msgId, tick, crc, then the base haloRAW_INPUT_X360 fields (buttons,
// sThumbLX, sThumbLY, sThumbRX, sThumbRY), byte-reordering each field first when the stream is in
// reordering mode.
void haloRAW_INPUT_X360_ENV::Pack(ds::BIT_STREAM &stream, const dsDATA &userCtx) const
{
    int msgIdField = msgId;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&msgIdField), 4, 4);
    stream.RawWrite(&msgIdField, 0x20u, 0);

    int tickField = tick;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&tickField), 4, 4);
    stream.RawWrite(&tickField, 0x20u, 0);

    int crcField = crc;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&crcField), 4, 4);
    stream.RawWrite(&crcField, 0x20u, 0);

    unsigned int buttonsField = buttons;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&buttonsField), 4, 4);
    stream.RawWrite(&buttonsField, 0x20u, 0);

    __int16 thumbLX = sThumbLX;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&thumbLX), 2, 2);
    stream.RawWrite(&thumbLX, 0x10u, 0);

    __int16 thumbLY = sThumbLY;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&thumbLY), 2, 2);
    stream.RawWrite(&thumbLY, 0x10u, 0);

    __int16 thumbRX = sThumbRX;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&thumbRX), 2, 2);
    stream.RawWrite(&thumbRX, 0x10u, 0);

    __int16 thumbRY = sThumbRY;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&thumbRY), 2, 2);
    stream.RawWrite(&thumbRY, 0x10u, 0);
}
