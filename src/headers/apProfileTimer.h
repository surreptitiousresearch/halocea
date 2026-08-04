#pragma once
/* apProfileTimer — ws-engine performance-counter timer: raw start / accumulated tick counts
 * (osGetPerfCounter units). DB-verified layout. */

typedef struct apProfileTimer
{
    unsigned long long startTime; /* 0x0 */
    unsigned long long totalTime; /* 0x8 */
#ifdef __cplusplus
    // 0x823CDEA0 — convert accumulated ticks to seconds and reset the accumulator.
    float GetResSec();
#endif
} apProfileTimer;
