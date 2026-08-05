#include "../../headers/ws/snd/SYSTEM_FMOD.h"
#include "../../headers/ws/snd/SYSTEM.h"
#include "../../headers/ws/snd/BUFFER.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// ---- boundary declarations ----------------------------------------------------------------
// FMOD Ex SDK — current sound-heap allocation stats (blocking==true => precise). boundary.
extern "C" void FMOD_Memory_GetStats(int *currentAllocated, int *maxAllocated, bool blocking);

// os-layer helpers (real bodies in src/ws/os/). boundary.
extern "C" int  osGetCurThreadId();
extern "C" void osOutputDebugString(const char *format, ...);
extern "C" unsigned int osGetTime();
extern "C" void osDebugBreak();

// Saber log sink. boundary.
extern void _apLog(const char *format, ...);

// Debug-tagged operator new (size, file, line) the ws build links in. boundary.
// First param must be size_t per [basic.stc.dynamic]; the PPC target's size_t is 32-bit but the
// x64 syntax gate requires the standard size_t spelling.
void *operator new(size_t size, const char *file, unsigned int line);

// ws-engine debug-console bool toggles that suppress new sounds. dbgVAR_SIMPLE<bool,1>; only the
// `.value` byte is read here. boundary — owned by the src/ws/snd drain.
struct SND_DBGVAR_BOOL { unsigned char _dbgVarBase[12]; unsigned char value; };
extern SND_DBGVAR_BOOL dbg_disableSounds;
extern SND_DBGVAR_BOOL dbg_disableNewSounds;

namespace snd {
// FMOD backend buffer bring-up. boundary — bodies in the src/ws/snd drain.
extern void BUFFER_FMOD_construct(BUFFER_FMOD *self);                    // snd::BUFFER_FMOD::BUFFER_FMOD
extern bool BUFFER_FMOD_Init(BUFFER_FMOD *self, int eventIdx,
                             const BUFFER_PARAMS &params);              // snd::BUFFER_FMOD::Init
}

// 0x82E36928 (snd::SYSTEM_FMOD::GetBuffer, D:\Projects\code\common\src.sys\drv\sound\snd_fmod.cpp).
// Resolve `name` to an FMOD event and start it. Guards, in order: backend must be inited; must run
// on the owning sound thread; FMOD sound heap must be under 25 MiB; the two debug-disable toggles;
// per-sound break/skip debug filters; and the name must exist in the loaded .fev. On success a
// BUFFER_FMOD is allocated, initialised, ref-counted, registered in `buffers` (and `virtualEvents`
// if it started virtual), and returned as snd::BUFFER*. Any failure returns nullptr.
snd::BUFFER *snd::SYSTEM_FMOD::GetBuffer(dsSTRID name, const snd::BUFFER_PARAMS &params)
{
    // `state` is the snd::SYSTEM base's dsFLAGS; SST_INITED (bit 0) means the backend is up.
    if ((this->state.val & SST_INITED) == 0)
        return nullptr;

    // Off-thread call guard — only meaningful once the driver singleton itself is live.
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != (unsigned int)osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "snd::SYSTEM_FMOD::GetBuffer", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != (unsigned int)osGetCurThreadId())
            ((STRONG_ASSERT_DUMMY *)nullptr)->Crash("snd::THREAD_ID == osGetCurThreadId()",
                                                    "D:\\Projects\\code\\common\\src.sys\\drv\\sound\\snd_fmod.cpp",
                                                    1752,
                                                    empty_string);
        return nullptr;
    }

    // FMOD sound-heap budget: bail (only logging) once past 25 MiB.
    int currentAllocated = 0;
    FMOD_Memory_GetStats(&currentAllocated, nullptr, true);
    if (currentAllocated > 26214400)
    {
        osOutputDebugString("SKIP %s\n", name.CStr());
        return nullptr;
    }

    if (dbg_disableSounds.value || dbg_disableNewSounds.value)
        return nullptr;

    // Break-on-play filter (skipped here when play-stat printing is on; re-applied on failure).
    if (!this->dbg.PrintPlayEnabled())
    {
        if (this->dbg.IsBreakOn(name.CStr()))
            osDebugBreak();
    }

    if (this->dbg.IsSkipped(name.CStr()))
        return nullptr;

    snd::CMP_EQ cmp;
    cmp.name = name;
    int eventIdx = this->implFind<snd::CMP_EQ>(&cmp);
    if (eventIdx == -1)
    {
        _apLog("~Sound,Error~Sound \"%s\" hasn't been found in .fev", name.CStr());
        return nullptr;
    }

    snd::BUFFER_FMOD *buffer =
        (snd::BUFFER_FMOD *)operator new(0x70u,
                                         "D:\\Projects\\code\\common\\src.sys\\drv\\sound\\snd_fmod.cpp",
                                         0x6F8u);
    if (buffer)
        snd::BUFFER_FMOD_construct(buffer);

    // View the freshly built object through its snd::BUFFER base for the state/vtable accesses.
    snd::BUFFER *asBuffer = (snd::BUFFER *)buffer;

    // Success when Init succeeds OR the event legitimately started virtual (BST_VIRTUAL).
    if (snd::BUFFER_FMOD_Init(buffer, eventIdx, params) ||
        (asBuffer->state.val & BST_VIRTUAL) != 0)
    {
        const char *loopTag = (asBuffer->state.val & BST_LOOPED) != 0 ? "L " : empty_string;
        // Timestamp in seconds (millisecond clock wrapped at 100000 ms). The DB shows the usual
        // PPC varargs double-promotion packing around this float; modeled directly here.
        float seconds = (float)(osGetTime() % 100000u) * 0.001f;
        _apLog("~Sound~PLAY %s (%s%s%.3f s)", name.CStr(), loopTag, seconds);

        if ((asBuffer->state.val & BST_VIRTUAL) != 0)
            this->virtualEvents.PushBack(buffer);

        asBuffer->AddRef();
        this->buffers.PushBack(buffer);
        return asBuffer;
    }

    // Init failed: optionally break, then invoke the scalar-deleting destructor (BUFFER_vtbl
    // slot 18, offset +72) with deleteFlag=1 to free the object, and report no buffer.
    if (this->dbg.PrintPlayEnabled())
    {
        if (this->dbg.IsBreakOn(name.CStr()))
            osDebugBreak();
    }
    asBuffer->__vftable->dtr_BUFFER(asBuffer, 1); // scalar-deleting dtr: deleteFlag=1 => free
    return nullptr;
}
