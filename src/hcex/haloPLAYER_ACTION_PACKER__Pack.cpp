#include <stdint.h>
#include "../headers/hcex/haloPLAYER_ACTION_PACKER.h"
#include "../headers/ws/ds/dsDATA.h"

// 0x823BC330 — serialize each field of the wrapped haloPLAYER_ACTION into `stream`, in
// declaration order, byte-reordering each field first when the stream is in reordering mode.
void haloPLAYER_ACTION_PACKER::Pack(ds::BIT_STREAM &stream, const dsDATA &userCtx) const
{
    unsigned int flags = control_flags;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&flags), 4, 4);
    stream.RawWrite(&flags, 0x20u, 0);

    float facingYaw = desired_facing_yaw;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&facingYaw), 4, 4);
    stream.RawWrite(&facingYaw, 0x20u, 0);

    float facingPitch = desired_facing_pitch;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&facingPitch), 4, 4);
    stream.RawWrite(&facingPitch, 0x20u, 0);

    float throttleXField = throttle_x;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&throttleXField), 4, 4);
    stream.RawWrite(&throttleXField, 0x20u, 0);

    float throttleYField = throttle_y;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&throttleYField), 4, 4);
    stream.RawWrite(&throttleYField, 0x20u, 0);

    float primaryTriggerField = primary_trigger;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&primaryTriggerField), 4, 4);
    stream.RawWrite(&primaryTriggerField, 0x20u, 0);

    int16_t weaponIndex = desired_weapon_index;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&weaponIndex), 2, 2);
    stream.RawWrite(&weaponIndex, 0x10u, 0);

    int16_t grenadeIndex = desired_grenade_index;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&grenadeIndex), 2, 2);
    stream.RawWrite(&grenadeIndex, 0x10u, 0);

    int16_t zoomLevel = desired_zoom_level;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&zoomLevel), 2, 2);
    stream.RawWrite(&zoomLevel, 0x10u, 0);
}
