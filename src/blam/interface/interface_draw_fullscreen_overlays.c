/* interface_draw_fullscreen_overlays @0x836A7650 — draw the full-screen interface layers on top of the
 * rendered scene, in order: cinematic letterbox, split-screen dividers, the HUD render timer, the loading
 * UI, the developer terminal, and the framerate display.
 *
 * DEVIATION: the decompiler threads a return value through these calls and types the function as int;
 * the disassembly shows six bare bl calls with no argument setup and the result is ignored by callers, so
 * they are reproduced as void calls. */

extern void cinematic_render(void);
extern void interface_splitscreen_render(void);
extern void hud_render_timer(void);
extern void ui_loading_update_and_render(void);
extern void terminal_draw(void);
extern void main_framerate_render(void);

void interface_draw_fullscreen_overlays(void)
{
    cinematic_render();
    interface_splitscreen_render();
    hud_render_timer();
    ui_loading_update_and_render();
    terminal_draw();
    main_framerate_render();
}
