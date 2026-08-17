/* ?RenderIngameStat@UI_STAT_SYS@ui_new@@QAAXXZ @0x82CF0528 */
#include "../../headers/ws/ui_new/UI_STAT_SYS.h"
#include "../../headers/ws/ui_new/ui_stat_boundary.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// Function-local static timer counter (see UI_STAT_SYS::RenderUIStat for the pattern).
static apCOUNTER_TIME_CALL localTimeCounter_RenderIngameStat("ui/new/UI_STAT_SYS::RenderIngameStat");

// 0x82CF0554 — draw every queued ingame debug-print (world/screen-anchored text, `ingameParams`
// shared by all of them) whose anchor falls inside `viewport`. Each item's ASCII text is widened
// via gsSTRINGS::AsciiToUnicode into a scratch wide buffer before printing (INGAME_ITEM::StatData
// is ASCII; UI_STAT_SYS::Print takes wide text). Like RenderUIStat, a failed one-time texture
// init latches `switchOff_0` and permanently no-ops thereafter.
void ui_new::UI_STAT_SYS::RenderIngameStat()
{
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> timerScope(&localTimeCounter_RenderIngameStat);

    if (switchOff_0)
        return;

    if (!ingameRenderer.pTex.ptr)
    {
        bool inited = ingameRenderer.InitTexture(font.m_TextureName);
        if (!inited)
        {
            switchOff_0 = true;
            return;
        }
    }

    // Lazily-allocated-once wide scratch buffer used to widen each item's ASCII text.
    static dsTSTRING<wchar_t> buf;

    for (int i = 0; i < ingameItemsForRender.nElem; ++i)
    {
        INGAME_ITEM &item = ingameItemsForRender[i];
        float x = (float)item.x;
        float y = (float)item.y;

        if (x >= viewport.l && y >= viewport.t && x < viewport.r && y < viewport.b)
        {
            if (!IGNORE_STRONG_ASSERT && !gsStrings)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("gsStrings", "D:\\Projects\\code\\common\\src.sys\\gm_sys\\ui\\ui_stat.cpp", 392, empty_string);

            int strLen = item.StatData.pBuffer->strLen;
            wchar_t *wbuf = buf.Lock(strLen + 1);
            gsStrings->AsciiToUnicode(item.StatData.pBuffer->str, wbuf, strLen + 1);

            m2dV pos = {x, y};
            Print(ingameRenderer, pos, buf.pBuffer->str, ingameParams);

            buf.Unlock();
        }
    }

    ingameRenderer.Flush();
}
