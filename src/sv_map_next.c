/* sv_map_next @0x83765D08 — console command stub: on this (non-dedicated) build it only prints that the
 * command is dedicated-server-only. */

typedef struct real_argb_color real_argb_color;
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_map_next(void)
{
    ship_terminal_printf(0, "sv_map_next is a dedicated server-only function!");
}
