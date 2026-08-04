/* ui_set_next_level @0x83730F20 — choose what happens after the current solo level: -1 rolls the credits,
 * an out-of-range level (>9) returns to the main menu, and a valid level sets the next solo map name and
 * disallows persistent storage for it. */

#include <stdint.h>

extern void main_roll_credits(void);
extern void main_goto_main_menu(void);
extern const char *main_get_solo_level_name(int16_t level);
extern void main_set_map_name(const char *name);
extern void main_disallow_persistent_storage(void);

void ui_set_next_level(int16_t level)
{
    if ( level == -1 )
    {
        main_roll_credits();
    }
    else if ( level < 0 || level > 9 )  /* disasm: two signed cmpwi (blt 0, bgt 9), not an unsigned range check */
    {
        main_goto_main_menu();
    }
    else
    {
        const char *solo_level_name = main_get_solo_level_name(level);
        main_set_map_name(solo_level_name);
        main_disallow_persistent_storage();
    }
}
