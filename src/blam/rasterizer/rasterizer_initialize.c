/* rasterizer_initialize @ 0x836A5CC0
   Allocates the global model ambient-reflection tint, then tail-calls the real
   rasterizer bring-up routine (_rasterizer_initialize). The decompiler renders
   the tail call as a recursive call to rasterizer_initialize(); the call site
   actually targets _rasterizer_initialize @ 0x8369BA60. */

#include "headers/real_argb_color.h"

extern void *game_state_malloc(const char *name, const char *type, int size);
extern int _rasterizer_initialize(void);

real_argb_color *global_rasterizer_model_ambient_reflection_tint;

/* DEVIATION: r3 at blr is only _rasterizer_initialize's threaded status (no own r3 def);
   sole caller shell_initialize (entry @0x836FBDA0, call site @0x836FBDC0) ignores it —
   void per the render_dispose rule. */
void rasterizer_initialize(void)
{
    global_rasterizer_model_ambient_reflection_tint =
        game_state_malloc("rasterizer model ambient reflection tint", 0, 16);
    _rasterizer_initialize();
}
