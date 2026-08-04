#pragma once
/* platoon_rule — an 8-byte AI rule expression evaluated by encounter_test_rule.
 * Only rule_type@0 and platoon_index@2 are read by the evaluator; the remaining 4 bytes are
 * unread here (likely the rule's threshold operands, baked into the rule_type test). */

typedef struct platoon_rule
{
    __int16       rule_type;      /* 0x00 — 1..9 strength/count test selector */
    __int16       platoon_index;  /* 0x02 — platoon within the encounter, or <0 for encounter-wide */
    int           pad;            /* 0x04 */
} platoon_rule;
