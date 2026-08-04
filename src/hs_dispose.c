/* hs_dispose @ 0x8372ED10 — HaloScript runtime + object-list teardown */
extern void hs_runtime_dispose_from_old_map(void);
extern void object_lists_dispose(void);
void hs_dispose(void)
{
    hs_runtime_dispose_from_old_map();
    object_lists_dispose();
}
