#pragma once

#include "ai_allegiance_information_data.h"
#include "ai_combat_stimulus_information_data.h"
#include "ai_target_knowledge_information_data.h"

/* ai_information_data — 8-byte payload attached to an ai_communication_event, interpreted per event type as one
 * of several sub-structures (database anonymous union _10E88A240CE092642D185C1B92DC9791, named ___u0). $-name
 * kept verbatim ($ in identifiers is an MSVC extension), matching the mtlPARAM.h convention.
 *
 * DEVIATION: IDA reports the union as 6 bytes (its widest arm) because it does not round a union up to its
 * own alignment; it reports the containing struct as 8. A C union of a 4-byte-aligned arm IS 8, so the
 * struct is the union and nothing else. This header used to add an explicit `_pad6[2]` on top of that,
 * which measured 12 and shifted every consumer — unit_speech_item, unit_speech, _unit_datum, unit_datum,
 * biped_datum and vehicle_datum were all 4-to-8 bytes long as a result. */

typedef union _10E88A240CE092642D185C1B92DC9791
{
    ai_allegiance_information_data       allegiance;        /* event type 8 (allegiance change) */
    ai_combat_stimulus_information_data  combat_stimulus;
    ai_target_knowledge_information_data target_knowledge;
} _10E88A240CE092642D185C1B92DC9791;

typedef struct ai_information_data
{
    _10E88A240CE092642D185C1B92DC9791 ___u0; /* 0x00 */
} ai_information_data;             /* 8 bytes */
