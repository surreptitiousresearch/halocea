/* global_acknowledgement_speeds @ 0x821280EC (.rdata, 32 bytes = 4 x 4 x int16_t) — how fast an
 * actor acknowledges a prop, selected by the actor's combat knowledge (row) and the prop's
 * perception level (column). actor_perception_update reads
 * global_acknowledgement_speeds[knowledge][prop->perception] and turns the result into an
 * awareness delta (never -> 0, non_combat/guard/combat -> the actor definition's three
 * runtime_awareness_delta values, instant -> 1.0).
 * Initializer reconstructed from the binary (big-endian words, hi/lo int16 halves):
 *   +0x00 0x00000000  +0x04 0x00010003 -> row 0 = { 0, 0, 1, 3 }
 *   +0x08 0x00000001  +0x0C 0x00020003 -> row 1 = { 0, 1, 2, 3 }
 *   +0x10 0x00000002  +0x14 0x00030004 -> row 2 = { 0, 2, 3, 4 }
 *   +0x18 0x00000003  +0x1C 0x00040004 -> row 3 = { 0, 3, 4, 4 }
 * The 4 x 4 shape is the sole consumer's declaration and matches both index spaces exactly
 * (NUMBER_OF_ACTOR_KNOWLEDGE_TYPES == number_of_actor_perception_types == 4; 32 / 2 == 16).
 */
#include <stdint.h>
#include "../headers/actor_knowledge.h"
#include "../headers/actor_perception_type.h"
#include "../headers/actor_acknowledgement_speed.h"

const int16_t global_acknowledgement_speeds[NUMBER_OF_ACTOR_KNOWLEDGE_TYPES][number_of_actor_perception_types] =
{
    /* [_actor_knowledge_noncombat] */
    { _actor_acknowledgement_never, _actor_acknowledgement_never,
      _actor_acknowledgement_non_combat, _actor_acknowledgement_combat },
    /* [_actor_knowledge_guard] */
    { _actor_acknowledgement_never, _actor_acknowledgement_non_combat,
      _actor_acknowledgement_guard, _actor_acknowledgement_combat },
    /* [_actor_knowledge_searching] */
    { _actor_acknowledgement_never, _actor_acknowledgement_guard,
      _actor_acknowledgement_combat, _actor_acknowledgement_instant },
    /* [_actor_knowledge_definite] */
    { _actor_acknowledgement_never, _actor_acknowledgement_combat,
      _actor_acknowledgement_instant, _actor_acknowledgement_instant },
};
