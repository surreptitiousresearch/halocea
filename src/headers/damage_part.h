#ifndef DAMAGE_PART_H
#define DAMAGE_PART_H

/* Body-region index for the part a unit was last damaged/killed in.
 * DB anonymous enum: types_enum_values _D16934675E63AA98F29BD39A540AF42C (verbatim). */
enum damage_part
{
    _damage_part_gut            = 0,
    _damage_part_chest          = 1,
    _damage_part_head           = 2,
    _damage_part_left_shoulder  = 3,
    _damage_part_left_arm       = 4,
    _damage_part_left_leg       = 5,
    _damage_part_left_foot      = 6,
    _damage_part_right_shoulder = 7,
    _damage_part_right_arm      = 8,
    _damage_part_right_leg      = 9,
    _damage_part_right_foot     = 10,
    NUMBER_OF_DAMAGE_PARTS      = 11,
};

#endif /* DAMAGE_PART_H */
