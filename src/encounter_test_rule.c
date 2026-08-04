/* encounter_test_rule @0x8370CC0C — evaluate an AI platoon rule against an encounter's strength.
 * The rule targets either a specific platoon within the encounter (platoon_index in range) or the
 * encounter as a whole (platoon_index out of range / negative). It reads that target's original
 * count, current strength fraction, and current count, then applies the rule_type test. Rules only
 * fire for targets that ever had units (original_count > 0). Returns 1 if the rule's condition holds.
 *
 * All fields resolved to the DB encounter_datum/platoon_datum structs: platoon_base @+8,
 * platoon_count @+0xA, and the encounter-wide fallback strength state original_count @+0x18,
 * current_strength_fraction @+0x34, current_count @+0x2A. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/platoon_datum.h"
#include "headers/platoon_rule.h"
#include "headers/platoon_rule_type.h"
#include "headers/blam_data_globals.h"


uint8_t encounter_test_rule(uint16_t encounter_index, platoon_rule *rule)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    __int16 platoon_index = rule->platoon_index;

    __int16 original_count;
    float   current_strength_fraction;
    __int16 current_count;

    if (platoon_index >= 0 && platoon_index < encounter->platoon_count)
    {
        platoon_datum *platoon = &platoon_array[(__int16)(encounter->platoon_base + platoon_index)];
        original_count            = platoon->original_count;
        current_strength_fraction = platoon->current_strength_fraction;
        current_count             = platoon->current_count;
    }
    else
    {
        original_count            = encounter->original_count;
        current_strength_fraction = encounter->current_strength_fraction;
        current_count             = encounter->current_count;
    }

    if (original_count <= 0)
        return 0;

    /* The case 4..9 expressions are the decompiler's expansion of signed count comparisons; kept
     * verbatim as they are faithful and valid C. */
    switch (rule->rule_type)
    {
        case _platoon_rule_75_strength: /* strength below 75% */
            return current_strength_fraction < 0.75f;
        case _platoon_rule_50_strength: /* strength below 50% */
            return current_strength_fraction < 0.5f;
        case _platoon_rule_25_strength: /* strength below 25% */
            return current_strength_fraction < 0.25f;
        case _platoon_rule_anybody_dead:
            return (((original_count ^ current_count) >= 0)
                    + (current_count >= (unsigned int)original_count)) & 1;
        case _platoon_rule_25_dead:
            return original_count >= 4 * current_count / 3;
        case _platoon_rule_50_dead:
            return (unsigned __int8)((original_count >> 31)
                                    + (original_count >= (unsigned int)(2 * current_count))
                                    + ((current_count & 0x40000000) != 0));
        case _platoon_rule_75_dead:
            return (unsigned __int8)((original_count >> 31)
                                    + (original_count >= (unsigned int)(4 * current_count))
                                    + ((current_count & 0x20000000) != 0));
        case _platoon_rule_all_but_one_dead:
            return (unsigned __int8)(((unsigned int)current_count <= 1) + (current_count < 0));
        case _platoon_rule_all_dead: /* wiped out */
            return current_count == 0;
        default:
            return 0;
    }
}
