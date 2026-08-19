/* reply_filter_fighting @0x837CC1E8 — tail-call forwarder (`mr r3, r5; b actor_is_fighting`; confirmed via
 * disasm, not a bare blr). Discards `original_unit_index`/`communication` and forwards only
 * `reply_actor_index` (the 3rd argument, register r5) to actor_is_fighting, returning its result
 * directly. actor_is_fighting itself is not yet decompiled — declared extern with its own DB prototype. */

#include <stdint.h>
#include "headers/ai_information_packet.h"

extern uint8_t actor_is_fighting(int actor_index);

uint8_t reply_filter_fighting(int original_unit_index, ai_information_packet *communication, int reply_actor_index)
{
    return actor_is_fighting(reply_actor_index);
}
