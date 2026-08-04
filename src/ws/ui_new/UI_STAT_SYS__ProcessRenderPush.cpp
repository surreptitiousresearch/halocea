#include "../../headers/ws/ui_new/UI_STAT_SYS.h"
#include "../../headers/ws/ui_new/ui_stat_boundary.h"

// 0x82CEEF40 — per-frame housekeeping run from the render-push phase (before RenderUIStat/
// RenderIngameStat draw): refresh every registered UI-stat line's text via its callback, then
// (if ingame debug text is enabled) swap the ingame item double-buffer so this frame's queued
// items become the ones RenderIngameStat will draw, resetting the now-free back buffer and
// snapshotting the shared ingame text params from their debug-toggle globals.
void ui_new::UI_STAT_SYS::ProcessRenderPush()
{
    if (!IGNORE_STRONG_ASSERT && lock.lockDepth > 0)
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash("! lock.IsLocked()", "D:\\Projects\\code\\common\\src.sys\\gm_sys\\ui\\ui_stat.cpp", 276, dsStrongAssertMessage);
    lock.Lock("D:\\Projects\\code\\common\\src.sys\\gm_sys\\ui\\ui_stat.cpp", 277);

    for (int i = 0; i < uiStatItems.nElem; ++i)
    {
        UI_STAT_ITEM &item = uiStatItems[i];
        item.StatData.Clear();
        item.GetStatDataCB(&item.StatData);
    }

    if (dbgIngameTextEnable.value)
    {
        if (ingameItems.nElem > 0 || ingameItemsForRender.nElem > 0)
        {
            ingameItems.Swap(ingameItemsForRender);
            ingameItems.Clear();

            ingameParams.color = dbgIngameTextColor.value;
            ingameParams.scale = dbgIngameTextScale.value;
            ingameParams.alignment = 68; // 0x44 = center-h (4) | vcenter (0x40)
            ingameParams.hasShadow = dbgIngameTextShadow.value;
        }
    }
    else
    {
        ingameItems.Clear();
        ingameItemsForRender.Clear();
    }

    lock.Unlock("D:\\Projects\\code\\common\\src.sys\\gm_sys\\ui\\ui_stat.cpp", 277);
}
