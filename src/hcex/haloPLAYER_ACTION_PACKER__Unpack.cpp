#include "../headers/hcex/haloPLAYER_ACTION_PACKER.h"
#include "../headers/ws/ds/dsDATA.h"

// 0x823BC5A0 — deserialize each field of the wrapped haloPLAYER_ACTION from `stream` in the same
// field order as Pack, byte-reordering in place when the stream is in reordering mode.
void haloPLAYER_ACTION_PACKER::Unpack(ds::BIT_STREAM &stream, const dsDATA &userCtx)
{
    stream.RawRead(&control_flags, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&control_flags), 4, 4);

    stream.RawRead(&desired_facing_yaw, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&desired_facing_yaw), 4, 4);

    stream.RawRead(&desired_facing_pitch, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&desired_facing_pitch), 4, 4);

    stream.RawRead(&throttle_x, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&throttle_x), 4, 4);

    stream.RawRead(&throttle_y, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&throttle_y), 4, 4);

    stream.RawRead(&primary_trigger, 0x20u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&primary_trigger), 4, 4);

    stream.RawRead(&desired_weapon_index, 0x10u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&desired_weapon_index), 2, 2);

    stream.RawRead(&desired_grenade_index, 0x10u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&desired_grenade_index), 2, 2);

    stream.RawRead(&desired_zoom_level, 0x10u, 0);
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<unsigned __int8 *>(&desired_zoom_level), 2, 2);
}
