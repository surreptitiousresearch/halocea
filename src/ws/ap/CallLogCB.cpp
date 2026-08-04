/* CallLogCB @ 0x825C86E0 -- ap_log.cpp (D:\Projects\code\common\src.sys\ap\ap_log.cpp).
 * Fans a formatted log line out to every registered log callback in logsCB[LOG_CALLBACKS_COUNT].
 * Called by apLogV()/apLogDV() once a log message has been formatted, to dispatch it to whatever
 * sinks (console, file, network) are currently subscribed.
 *
 * DEVIATION: `logsCB_0` is not a second global -- it is the DB's synthetic name for the address
 * immediately past the 4-element logsCB[] array (i.e. &logsCB[4]); the loop bound is reproduced
 * here as `idx < LOG_CALLBACKS_COUNT` directly instead of a second extern.
 */

#define LOG_CALLBACKS_COUNT 4

extern void (*logsCB[LOG_CALLBACKS_COUNT])(const char *text); /* 0x842090F8 */
extern int  isFirstTime;             /* 0x8413F1DC -- one-shot logsCB[] zero-init guard */
extern int  IGNORE_STRONG_ASSERT;    /* boundary -- disables the strong-assert crash path */
extern const char empty_string[]; /* shared "" @0x8200155A */

/* boundary -- STRONG_ASSERT_DUMMY::Crash(this, condition, file, line, info); see
 * src/dlDumpNoMemLeft.c for the established signature/call convention this mirrors. */
void STRONG_ASSERT_DUMMY_Crash(void *unused, const char *condition, const char *file, int line, const char *info);

void CallLogCB(const char *text)
{
    if (isFirstTime)
    {
        logsCB[0] = 0;
        logsCB[1] = 0;
        logsCB[2] = 0;
        logsCB[3] = 0;
        isFirstTime = 0;
    }

    for (int idx = 0; idx < LOG_CALLBACKS_COUNT; ++idx)
    {
        if (!IGNORE_STRONG_ASSERT && idx >= LOG_CALLBACKS_COUNT)
            STRONG_ASSERT_DUMMY_Crash(
                0,
                "idx < LOG_CALLBACKS_COUNT",
                "D:\\Projects\\code\\common\\src.sys\\ap\\ap_log.cpp",
                86,
                empty_string);

        if (logsCB[idx])
            logsCB[idx](text);
    }
}
