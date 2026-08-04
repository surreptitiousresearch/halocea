/* errors_dispose @0x83764B28 — tail-call forwarder (`b stack_walk_dispose`; confirmed via disasm, not
 * a bare blr). stack_walk_dispose itself is not yet decompiled (no DB prototype recorded either —
 * modeled void(void) per the project's established X_dispose no-arg convention) — declared extern
 * and called through. */

extern void stack_walk_dispose(void);

void errors_dispose(void)
{
    stack_walk_dispose();
}
