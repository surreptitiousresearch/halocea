/* hs_initialize @ 0x837303B0 — HaloScript runtime init */

extern void object_lists_initialize(void);
extern void hs_runtime_initialize(void);
extern void hs_initialize_for_new_map(void);

void hs_initialize(void)
{
    object_lists_initialize();
    hs_runtime_initialize();
    hs_initialize_for_new_map(); /* attested void: r3-thread + phantom arg dropped */
}
