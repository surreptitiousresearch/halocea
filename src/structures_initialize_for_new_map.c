/* structures_initialize_for_new_map @0x83744370 — reset structure detail objects then structure decals. */

extern void structure_detail_objects_initialize_for_new_map(void);
extern void structure_decals_initialize_for_new_map(void);

void structures_initialize_for_new_map(void)
{
    structure_detail_objects_initialize_for_new_map();
    structure_decals_initialize_for_new_map();
}
