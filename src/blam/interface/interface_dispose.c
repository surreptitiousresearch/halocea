/* interface_dispose @ 0x836A6470 — one-time interface teardown. Four independent no-arg subsystem
 * disposers in sequence (the decompiler's threaded return value between them is an r3 artifact; disasm
 * shows no argument setup and all four callees are void). */
extern void draw_string_dispose(void);
extern void terminal_dispose(void);
extern void hud_dispose(void);
extern void first_person_weapons_dispose(void);
void interface_dispose(void)
{
    draw_string_dispose();
    terminal_dispose();
    hud_dispose();
    first_person_weapons_dispose();
}
