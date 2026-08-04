#include "../../headers/ws/ui_new/UI_STAT_SYS.h"
#include "../../headers/ws/ui_new/ui_stat_boundary.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// Function-local static timer counter + its RAII scope guard (apCOUNTER_TIME_CALL_WRP), matching
// the lazily-constructed-once-then-atexit-destroyed idiom used throughout this corpus for
// per-function profiling counters (see e.g. ui_new::UI_PROXY::ProcessRender).
static apCOUNTER_TIME_CALL localTimeCounter_RenderUIStat("ui/new/UI_STAT_SYS::RenderUIStat");

// 0x82CF02FC — draw every registered UI-stat line (top-of-screen debug counter list) into
// uiStatRenderer and flush it in one batch. If the shared glyph-atlas texture can't be
// initialised the first time this runs, `switchOff` latches true and the system permanently
// no-ops (this build never retries).
void ui_new::UI_STAT_SYS::RenderUIStat()
{
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> timerScope(&localTimeCounter_RenderUIStat);

    if (!IGNORE_STRONG_ASSERT && lock.lockDepth > 0)
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash("! lock.IsLocked()", "D:\\Projects\\code\\common\\src.sys\\gm_sys\\ui\\ui_stat.cpp", 325, dsStrongAssertMessage);
    lock.Lock("D:\\Projects\\code\\common\\src.sys\\gm_sys\\ui\\ui_stat.cpp", 326);

    if (!switchOff)
    {
        bool ready = true;
        if (!uiStatRenderer.pTex.ptr)
        {
            bool inited = uiStatRenderer.InitTexture(font.m_TextureName);
            ready = inited;
        }

        if (ready)
        {
            for (int i = 0; i < uiStatItems.nElem; ++i)
            {
                UI_STAT_ITEM &item = uiStatItems[i];
                if (item.StatData.pBuffer->strLen > 0)
                {
                    UI_STAT_PARAMS params;
                    params.color = {255.0f, 0.0f, 0.0f, 255.0f};
                    params.alignment = 0;
                    params.scale = 5.0f;
                    params.interval = 0.0f;
                    params.hasShadow = false;
                    item.GetStatParamsCB(&params);

                    m2dV pos = GetPosFromViewport(item.StatData.pBuffer->str, (char)params.alignment, params.scale);
                    Print(uiStatRenderer, pos, item.StatData.pBuffer->str, params);
                }
            }
            uiStatRenderer.Flush();
        }
        else
        {
            switchOff = true;
        }
    }

    lock.Unlock("D:\\Projects\\code\\common\\src.sys\\gm_sys\\ui\\ui_stat.cpp", 326);
}
