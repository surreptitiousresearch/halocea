/* hcex_plr_is_act_camo @0x83681AF0 — is the given local player's unit currently in active camouflage?
 * Resolves the local player -> player datum -> unit object index (player+0x34), verifies the object is a
 * unit (type mask 3), and tests its unit flags (dword 129 == +0x204) for the active-camo bit 0x10. */

#include <stdint.h>
#include "../headers/data_array.h"
#include "../headers/object_type.h"
#include "../headers/player_datum.h"
#include "../headers/unit_datum.h"
#include "../headers/unit_flags.h"

extern data_array *player_data;
extern int16_t local_player_count(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

extern "C" int hcex_plr_is_act_camo(int idx)
{
    if ( idx >= local_player_count() )
        return 0;

    int player_index = local_player_get_player_index((__int16)idx);
    if ( player_index == -1 )
        return 0;

    int unit_object_index =
        DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;

    unit_datum *unit = (unit_datum *)object_try_and_get_and_verify_type(unit_object_index, object_mask_unit);
    if ( !unit )
        return 0;

    return (unit->unit.flags & (1u << _unit_active_camouflaged_bit)) != 0;   /* active-camo bit */
}
