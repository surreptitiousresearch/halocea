#ifndef HS_SLEEP_H
#define HS_SLEEP_H

/* HaloScript thread sleep_until sentinel values.
 * Source: DB anonymous enum _42D661599D44F013495336376B8E46EE (types_enum_values, verbatim).
 * A non-negative sleep_until is an absolute game-time wake tick; 0 means running/awake. */
enum
{
    HS_SLEEP_FINISHED   = -1,   /* not sleeping (thread runnable) */
    HS_SLEEP_INDEFINITE = -2    /* dormant / sleep with no scheduled wake */
};

#endif /* HS_SLEEP_H */
