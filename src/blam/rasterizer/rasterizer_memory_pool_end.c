/* rasterizer_memory_pool_end @0x83791E30 — no-op on this platform (single `blr`). The PC rasterizer did per-frame
 * work here; the Xbox 360 path has nothing to tear down/begin, so the function is empty. */

void rasterizer_memory_pool_end(void)
{
}
