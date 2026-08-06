#pragma once
#include <stdint.h>
#include "ai_index_scope.h"

/* ai_index — the packed HaloScript "ai" reference. Big-endian byte order within the stored word:
 *
 *   byte 0  (bits 30-31) : ai_index_scope
 *   byte 1  (bits 16-23) : platoon index (scope _ai_index_platoon) or squad index (_ai_index_squad)
 *   bytes 2-3 (bits 0-15): encounter index
 *
 * Disassembly basis (ai_scripting_vehicle_encounter @0x83772098; the identical three encodings appear in
 * every ai_index consumer -- ai_index_*_iterator_new, ai_scripting_attach_unit(s)/count_internal/erase/
 * place/spawn_actor, actor_action_handle_vehicle_entry):
 *
 *   scope      0x83772110  srwi  r11, r4, 30      = rlwinm SH=2  MB=30 ME=31 (word 0x548B17BE)
 *                                                   a LOGICAL shift -- the packed value is unsigned
 *   sub-index  0x83772130  extrwi r10, r4, 8, 8   = rlwinm SH=16 MB=24 ME=31 (word 0x548A863E)
 *                                                   rotate-left-16 + keep low 8 = (x >> 16) & 0xFF
 *   encounter  0x837720E4  extsh / clrlwi ...,16  = the low halfword
 *
 * The sub-index shift is the one worth stating explicitly: Hex-Rays renders it BYTE1(x), which is
 * ((uint8_t *)&x)[1] -- byte 1 of a BIG-endian word, i.e. bits 16-23. Expanding BYTE1 with the
 * little-endian value form ((x >> 8) & 0xFF) selects the wrong byte on this target.
 *
 * PRODUCER-SIDE CONFIRMATION (stronger than any consumer disasm): src/ai_index_from_string.c:68
 * builds the word as `((sub << 16) & 0xFF0000) | type_flag | encounter_index`, and
 * src/encounter_squad_timer_expire.c:24 builds `(squad_index << 16) & 0xFF0000 | 0x80000000 |
 * encounter_index`. Bits 16-23 is settled by the encoder, not inferred from the decoders.
 *
 * ENCOUNTER EXTRACT -- TWO SPELLINGS, AND THEY ARE NOT INTERCHANGEABLE.
 * The corpus does the low-halfword extract two different ways and the difference is live:
 *   * `extsh` (SIGN-extend)  -- ai_scripting_vehicle_encounter @0x837720E4, which compares the
 *     result `>= 0` and assigns it to an int16_t. Use AI_INDEX_ENCOUNTER_SIGNED.
 *   * `clrlwi rX,rY,16` (ZERO-extend) -- every one of the ten ai_index_ and ai_scripting_ files
 *     drained 2026-08-06, all spelled (uint16_t). Use AI_INDEX_ENCOUNTER.
 * Substituting the signed form into a zero-extending site flips encounter indices 0x8000-0xFFFF
 * negative. An earlier revision of this header offered only the signed spelling under the plain
 * name; the agent draining those ten files correctly REFUSED to apply it corpus-wide rather than
 * inject that defect. Pick the spelling the callee's disassembly actually shows. */

#define AI_INDEX_SCOPE(ai_index)            ((ai_index) >> 30)
#define AI_INDEX_SUB_INDEX(ai_index)        (((ai_index) >> 16) & 0xFF)
/* zero-extending low halfword -- `clrlwi rX,rY,16`; the common spelling */
#define AI_INDEX_ENCOUNTER(ai_index)        ((uint16_t)(ai_index))
/* sign-extending low halfword -- `extsh`; only where the disasm shows it */
#define AI_INDEX_ENCOUNTER_SIGNED(ai_index) ((int16_t)(ai_index))
