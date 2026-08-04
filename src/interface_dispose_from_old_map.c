/* interface_dispose_from_old_map @ 0x836A6448 — per-map interface teardown. Three independent no-arg
 * subsystem disposers in sequence (the decompiler's threaded return value between them is an r3 artifact;
 * disasm shows no argument setup and all three callees are void). */
extern void draw_string_dispose_from_old_map(void);
extern void hud_dispose_from_old_map(void);
extern void first_person_weapons_dispose_from_old_map(void);
void interface_dispose_from_old_map(void)
{
    draw_string_dispose_from_old_map();
    hud_dispose_from_old_map();
    first_person_weapons_dispose_from_old_map();
}
