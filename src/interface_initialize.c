/* interface_initialize @ 0x836A6418 */

extern void terminal_initialize(void);
extern void hud_initialize(void); /* attested void */
extern void draw_string_initialize(void);
extern void first_person_weapons_initialize(void);

void interface_initialize(void)
{
    terminal_initialize();
    hud_initialize(); /* attested void: r3-thread + phantom arg dropped */
    draw_string_initialize();
    first_person_weapons_initialize(); /* attested void: r3-thread + phantom arg dropped */
}
