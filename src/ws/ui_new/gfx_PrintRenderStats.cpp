#include "../../headers/ws/ui_new/GFX_SYSTEM.h"
#include "../../headers/ws/ui_new/gfx_render_stats.h"
#include "../../headers/ws/ui_new/ui_stat_boundary.h"

// 0x8331C774 — format the GFx renderer's current counters into a string and print it to the
// given debug page. DEVIATION: the decompiled call site shows several extra registers (`v4..v13`)
// forwarded into wb::Printf's variadic slots, but none of them are ever assigned by this
// function (dead/uninitialized reads) — wb::Printf is passed a plain already-formatted string
// with no embedded %-specifiers, so this reconstructs it as a single-argument call.
void ui_new::gfx_PrintRenderStats(int page)
{
    if (gGfxSystem)
    {
        dsTSTRING<char> text;
        text.UnsafeInitEmpty(); // match the original's lazy-empty-singleton init before the call
        gGfxSystem->GetCountersString(&text);
        wb::Printf(page, text.pBuffer->str);
    }
}
