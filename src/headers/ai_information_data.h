#pragma once

#include "ai_allegiance_information_data.h"
#include "ai_combat_stimulus_information_data.h"
#include "ai_target_knowledge_information_data.h"

/* ai_information_data — 8-byte payload attached to an ai_communication_event, interpreted per event type as one
 * of several sub-structures (database anonymous union _10E88A240CE092642D185C1B92DC9791, named ___u0, sizeof 6,
 * with 2 trailing padding bytes to reach the struct's 8-byte, 4-byte-aligned size). $-name kept verbatim
 * ($ in identifiers is an MSVC extension), matching the mtlPARAM.h convention. */

typedef union _10E88A240CE092642D185C1B92DC9791
{
    ai_allegiance_information_data       allegiance;        /* event type 8 (allegiance change) */
    ai_combat_stimulus_information_data  combat_stimulus;
    ai_target_knowledge_information_data target_knowledge;
} _10E88A240CE092642D185C1B92DC9791;

typedef struct ai_information_data
{
    _10E88A240CE092642D185C1B92DC9791 ___u0; /* 0x00 (6 bytes) */
    unsigned char _pad6[2];                  /* 0x06 — trailing alignment pad */
} ai_information_data;             /* 8 bytes */
