/* first_person_weapons_initialize_for_new_map @0x8369DDB8 — clear both local players' first-person weapon
 * viewmodel state, marking each as having no owning unit and no active sound. */

#include <string.h>
#include "headers/first_person_weapon.h"

/* ATTEST: return is void, not void*. The decompiler threaded memset's r3 result into a return value, but
 * disasm shows the epilogue is a plain __restgprlr tail and 0/1 callers consume r3. */
void first_person_weapons_initialize_for_new_map(void)
{
    for ( int i = 0; i < 2; ++i )
    {
        first_person_weapon *weapon = &first_person_weapons[i];
        memset(weapon, 0, sizeof(first_person_weapon));
        weapon->unit_index = -1;
        weapon->current_sound_index = -1;
        weapon->current_sound_state = -1;
    }
}
