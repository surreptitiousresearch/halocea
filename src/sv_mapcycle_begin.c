/* sv_mapcycle_begin @0x83765CB8 — console command stub: on this (non-dedicated) build it only prints that
 * the command is dedicated-server-only. */

typedef struct real_argb_color real_argb_color;
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_mapcycle_begin(void)
{
    ship_terminal_printf(0, "sv_mapcycle_begin is a dedicated server-only function!");
}
