/* sv_mapcycle_add @0x83765CF4 — console command stub: dedicated-server-only on this build. */

typedef struct real_argb_color real_argb_color;
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_mapcycle_add(const char *map, const char *variant)
{
    ship_terminal_printf(0, "sv_mapcycle_add is a dedicated server-only function!");
}
