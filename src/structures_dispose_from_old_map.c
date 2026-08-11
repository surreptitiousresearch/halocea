/* structures_dispose_from_old_map @ 0x83744398 — structure decals + detail objects */
extern void structure_decals_dispose_from_old_map(void);
extern void structure_detail_objects_dispose_from_old_map(void);
void structures_dispose_from_old_map(void) /* attested void: tail-call of void fn, 0/1 callers consume r3 */
{
    structure_decals_dispose_from_old_map();
    structure_detail_objects_dispose_from_old_map();
}
