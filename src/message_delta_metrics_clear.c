/* message_delta_metrics_clear @0x837A07B0 — thunk forwarding to metrics_clear (resets the message-delta
 * metrics accumulators). */

extern void metrics_clear(void);

/* DEFECT FIX: metrics_clear is void (src/metrics_clear.c, @0x83818D20); this is a pure tail
 * call (b metrics_clear) so it inherits void, and the sole caller does not consume r3. */
void message_delta_metrics_clear(void)
{
    metrics_clear();
}
