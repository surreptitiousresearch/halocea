/* effect_get_random_seed @0x836E1248 — pick which random-number stream a live effect draws from.
 * Effects whose definition is flagged "must be deterministic" use the global
 * (networked / replay-stable) random seed; all others use the local seed. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/effect_definition.h"
#include "headers/effect_definition_flags.h"

extern uint32_t *get_global_random_seed_address(void);
extern uint32_t *get_global_local_random_seed_address(void);

uint32_t * effect_get_random_seed(int effect_definition_index)
{
    effect_definition *definition = TAG_GET(effect_definition, effect_definition_index);

    if ( (definition->flags & (1u << _effect_must_be_deterministic_bit)) != 0 )
        return get_global_random_seed_address();
    return get_global_local_random_seed_address();
}
