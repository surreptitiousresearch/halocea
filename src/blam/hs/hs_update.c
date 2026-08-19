/* hs_update @ 0x8372ED48 — thunk to the HaloScript runtime tick (tail `b hs_runtime_update`). */

extern void hs_runtime_update(void);

/* attested void (was int): tail call, callee returns void, 0 caller consumers */
void hs_update(void)
{
    hs_runtime_update();
}
