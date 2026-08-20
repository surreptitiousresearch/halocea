#ifndef HS_SLEEP_H
#define HS_SLEEP_H

/* HaloScript thread sleep_until sentinel values.
 * Source: DB anonymous enum _42D661599D44F013495336376B8E46EE (types_enum_values, verbatim).
 * A non-negative sleep_until is an absolute game-time wake tick; 0 means running/awake.
 * Both negative values park the thread: hs_runtime_update calls hs_thread_main only when
 * sleep_until >= 0 (cmpwi/blt @0x83690CC0-0x83690CC4), so a negative sleep_until is never run. */
enum
{
    HS_SLEEP_FINISHED   = -1,   /* thread has finished — parked, and hs_wake @0x8368D298 returns
                                   immediately on -1, so it cannot be resumed */
    HS_SLEEP_INDEFINITE = -2    /* dormant / sleep with no scheduled wake */
};

#endif /* HS_SLEEP_H */
