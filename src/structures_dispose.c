/* structures_dispose @ 0x837443C0 — structure decals + detail objects teardown */
extern void structure_decals_dispose(void);
extern void structure_detail_objects_dispose(void);
void structures_dispose(void) /* attested void: tail-call of void fn, 0/1 callers consume r3 */
{
    structure_decals_dispose();
    structure_detail_objects_dispose();
}
