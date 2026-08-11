#include "../../headers/ws/mdl/mdlLIP_SYNCER.h"

// File-scope lip-sync debug/config globals (defined in the lipsync translation unit).
namespace lipsync {
    extern int   __dbgPhonemeId;       // -1 = normal operation; otherwise force a single phoneme
    extern float __maxLipSyncBlendTime;
}

#include "../../headers/ws/ds/ds_assert_boundary.h"

// Boundary helpers (deeper than the 2-level descent).
extern float _m3dClamp(float lo, float hi, float val);

// mdlLIP_SYNCER::Update @ 0x827473E0
// Demangled signature is void Update(float dt); the a3..a6 params the decompiler shows are
// PPC/vararg ABI artifacts of the spline GetValue call sites and carry no real caller value.
void mdlLIP_SYNCER::Update(float dt)
{
    if (lipsync::__dbgPhonemeId != -1)
        return;

    STATUS status = this->status;
    if (status == OFF)
        return;

    if (status == EASE_ON) {
        float t = (float)(this->timeEaseCur + dt);
        this->timeEaseCur = this->timeEaseCur + dt;
        if (t > lipsync::__maxLipSyncBlendTime) {
            this->timeEaseCur = lipsync::__maxLipSyncBlendTime;
            this->status = ACTIVE;
        }
    } else if (status == EASE_OFF) {
        float t = (float)(this->timeEaseCur - dt);
        this->timeEaseCur = this->timeEaseCur - dt;
        if (t < 0.0f) {
            this->timeEaseCur = 0.0f;
            // decompiler read this->__vftable as mdlLIP_SYNCER_vtbl*; the base member is typed
            // mdlLIP_SYNC_IFACE_vtbl* here, so cast to reach the OnReset slot.
            mdlLIP_SYNCER_vtbl *vtbl = (mdlLIP_SYNCER_vtbl *)this->__vftable;
            this->status = OFF;
            vtbl->OnReset(this);
            return;
        }
    }

    float timeEnd = this->timeEnd;
    char advanced = 0;
    float timeLimit = (float)(this->timeEnd - 0.000001f);
    if (this->timeCur <= timeLimit) {
        float tNext = (float)(this->timeCur + dt);
        this->timeCur = this->timeCur + dt;
        advanced = 1;
        if (tNext >= timeLimit) {
            STATUS s = this->status;
            this->timeCur = timeEnd;
            if (s == ACTIVE || s == EASE_ON)
                this->status = EASE_OFF;
        }
    }

    if (!advanced)
        return;

    if (this->isClosedOnly) {
        dsCONST_ARRAY<PHONEME_INFO, 5> *seqList = &this->seqList;

        if (!IGNORE_STRONG_ASSERT && seqList->nEntry <= 4)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("IsValidIdx(idx)",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166, empty_string);

        m3dSPL *spline = this->seqList.list[4].spline;
        if (spline) {
            int rate = this->sampleRate;
            // m3dSPL vtable GetValue(self, t, out, aux): evaluate the spline at parameter t and
            // write the result to `out`. (The decompiler's extra int slots were PPC ABI padding
            // that lands the float t in f1 and the out ptr in r5 — see disasm 0x82A29710.)
            spline->__vftable->GetValue(
                spline,
                (float)((float)((float)rate * this->timeCur) * 0.0009765625f),
                &seqList->list[4].coef,
                nullptr);
        }
        seqList->list[4].coef = _m3dClamp(0.0f, 1.0f, seqList->list[4].coef);

        if (!IGNORE_STRONG_ASSERT && seqList->nEntry <= 0)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("IsValidIdx(idx)",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166, empty_string);

        this->seqList.list[0].coef =
            _m3dClamp(0.0f, 1.0f, (float)(1.0f - this->seqList.list[4].coef));
    } else {
        for (int i = 0; i <= 4; ++i) {
            PHONEME_INFO *info = &this->seqList[i]; // operator[] returns T&, take address
            if (info->spline) {
                int rate = this->sampleRate;
                info->spline->__vftable->GetValue(
                    info->spline,
                    (float)((float)((float)rate * this->timeCur) * 0.0009765625f),
                    &info->coef,
                    nullptr);
            }
        }
    }
}
