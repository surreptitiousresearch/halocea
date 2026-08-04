#ifndef DAMAGE_CATEGORY_H
#define DAMAGE_CATEGORY_H

/* damage category enum — headers_ref _FF789E00802E505FAD591F9FCAB6A17D */
enum damage_category
{
    _damage_category_none          = 0,
    _damage_category_falling       = 1,
    _damage_category_bullet        = 2,
    _damage_category_grenade       = 3,
    _damage_category_highexplosive = 4,
    _damage_category_sniper        = 5,
    _damage_category_melee         = 6,
    _damage_category_flame         = 7,
    _damage_category_mountedweapon = 8,
    _damage_category_vehicle       = 9,
    _damage_category_plasma        = 10,
    _damage_category_needle        = 11,
    _damage_category_shotgun       = 12,
    NUMBER_OF_DAMAGE_CATEGORIES    = 13,
};

#endif /* DAMAGE_CATEGORY_H */

// damage_category: DB-verified via types_enum_values _FF789E00802E505FAD591F9FCAB6A17D (anonymous compiled enum, names sans leading _)
