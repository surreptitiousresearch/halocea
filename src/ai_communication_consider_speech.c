/* ai_communication_consider_speech @0x837CC3F8 — test whether a unit can currently voice a line and adjust the
 * line's selection weight accordingly. Delegates to unit_test_speech (resolving the vocalization and sound
 * definition); a "queued" result (status 1) scales the weight by 0.3. When recent-line disabling is allowed
 * and this is a low-priority line (< 5) that the unit spoke before, it enforces a minimum gap: if too little
 * time has passed it rejects the line (weight 0, returns 0); within a 60-tick ramp window above the gap it
 * scales the weight linearly toward full.
 *
 * Deviation: Hex-Rays mis-modelled the unit_test_speech out-time as the high half of a packed __int64 and
 * flagged "local variable allocation failed"; reconstructed with plain locals from the documented prototype.
 * The gap threshold is communication_timer_tolerances[communication_priority][0][2] * 30 + delay_ticks. */

#include <stdint.h>
#include "headers/dialogue_usage.h"  /* not strictly needed, keeps tolerance-table context with the dispatcher */
#include "headers/ai_communication_priority.h"

extern const float communication_timer_tolerances[8][2][5];
extern int16_t unit_test_speech(int unit_index, int16_t priority, uint8_t allow_recursive_lookup, uint8_t allow_queue, int *unit_last_speech_time, int16_t *vocalization_type_reference, int *sound_definition_index_reference);
extern int game_time_get(void);

int16_t ai_communication_consider_speech(int unit_index, int16_t communication_priority, int16_t speech_priority,
        int16_t delay_ticks, uint8_t allow_vocalization_lookup, uint8_t allow_recent_disabling,
        int16_t *vocalization_type, float *weight, int *sound_definition_index_reference, char *debugstring)
{
    int last_speech_time = -1;
    int16_t status = unit_test_speech(unit_index, speech_priority, allow_vocalization_lookup, 1u,
                                      &last_speech_time, vocalization_type, sound_definition_index_reference);

    if ( status == 1 )
        *weight = *weight * 0.30000001f;

    if ( allow_recent_disabling && communication_priority < _ai_communication_priority_shout && last_speech_time != -1 )
    {
        int now = game_time_get();
        int elapsed_raw = now - last_speech_time;
        /* DEVIATION (2026-08-12, #134): was `__CFADD__(elapsed_raw, 0x80000000) ? 0 : ...` — the
         * Hex-Rays carry-out spelling of a clamp at zero, and the last live occurrence of that
         * intrinsic in the corpus. Disasm: `li r11,0 / xoris r8,r11,0x8000` builds 0x80000000,
         * `addc r6, r7, r8` @0x837CC4D4 takes the carry out of elapsed_raw + 0x80000000, and
         * `subfe r4, r5, r5` @0x837CC4D8 turns it into 0 / 0xFFFFFFFF for the following mask.
         * Carry is set exactly when (uint32_t)elapsed_raw >= 0x80000000, i.e. when elapsed_raw is
         * negative as a signed word — so the whole idiom is `max(elapsed_raw, 0)`. Same correction
         * as bitmap_draw_string.c and ai_communication_event.c already carry in comments. */
        int16_t elapsed = elapsed_raw < 0 ? 0 : (int16_t)elapsed_raw;
        int16_t threshold =
            (int16_t)(int)(communication_timer_tolerances[communication_priority][0][2] * 30.0f
                                  + (float)delay_ticks);
        if ( elapsed > threshold )
        {
            if ( elapsed < threshold + 60 )
                *weight = (float)(elapsed - threshold) * *weight * 0.016666668f;
        }
        else
        {
            *weight = 0.0f;
            return 0;
        }
    }
    return status;
}
