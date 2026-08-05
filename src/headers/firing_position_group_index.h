#pragma once

/* firing_position_group_index — a firing position's letter group (a..z), the bit index into
 * squad_definition.firing_position_groups[] masks. DB anonymous enum $DABB794FD63AD65B35FDA23295EDE258. */
enum firing_position_group_index {
    NUMBER_OF_FIRING_POSITION_GROUP_INDICES = 26,
    ALL_FIRING_POSITION_GROUPS = 0x3FFFFFF,
    CUSTOM_SAPIEN_FIRING_POINT_FIELD = 1667657830,
};

// firing_position_group_index: DB-verified via .sweep/enum_values.json $DABB794FD63AD65B35FDA23295EDE258
