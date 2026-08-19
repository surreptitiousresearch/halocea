/* weapon_stop_reload @0x836DCE88 — abort an in-progress reload on a weapon.
 * Thin wrapper: forwards directly to weapon_reset. */

extern void weapon_reset(int weapon_index);

void weapon_stop_reload(int weapon_index)
{
    weapon_reset(weapon_index);
}
