#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ds/dsBIT_ARRAY_IMPL.h"
#include "../../headers/ws/ai/aiWATCHER_props_boundary.h"
#include <math.h>

// AI_BODY_UNCONTROLLED_TIME_DISABLED: the "uncontrolled timer disabled" sentinel (-1.0f).
static const float AI_BODY_UNCONTROLLED_TIME_DISABLED = -1.0f;

// 0x832AC070 ?UpdateUncFall@aiBODY@@IAAXM@Z
// Refresh the uncontrolled (0x1000000) and falling (0x2000000) status body-flags from the property
// cache, advance the fall/uncontrolled timers, and notify the "best" module on control transitions.
void aiBODY::UpdateUncFall(float dt)
{
    if (this->pBrain->IsDead())
        return;

    // uncontrolled flag (bit 0x1000000)
    const ctrlPROP_LIST *cache = this->GetRefPropCache();
    int isUnc = dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_UNCONTROLLED.id, cache->props.array, 12);
    aiSTATUS *status = this->pBrain->st.pointee;
    {
        unsigned int val = (unsigned int)status->body.val;
        status->body.val = (int)(isUnc ? (val | 0x1000000u) : (val & 0xFEFFFFFFu));
    }

    // falling flag (bit 0x2000000)
    const ctrlPROP_LIST *cache2 = this->GetRefPropCache();
    int isFalling = dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_FALLING.id, cache2->props.array, 12);
    status = this->pBrain->st.pointee;
    {
        unsigned int val = (unsigned int)status->body.val;
        status->body.val = (int)(isFalling ? (val | 0x2000000u) : (val & 0xFDFFFFFFu));
    }

    aiBRAIN *pBrain = this->pBrain;
    if ((((unsigned int)pBrain->st.pointee->body.val >> 25) & 1) != 0)
        this->timeFalling = this->timeFalling + dt;
    else
        this->timeFalling = 0.0f;

    aiSTATUS *st = pBrain->st.pointee;
    unsigned int bodyVal = (unsigned int)st->body.val;
    float result;
    if ((bodyVal & 0x1000000u) != 0 || this->timeFalling > 0.5f)
    {
        result = AI_BODY_UNCONTROLLED_TIME_DISABLED;
        if (((bodyVal >> 26) & 1) == 0)
        {
            if (fabsf(AI_BODY_UNCONTROLLED_TIME_DISABLED - this->timeUncontrolled) < 0.000001f)
            {
                this->timeUncontrolled = 0.0f;
                pBrain->mind.best.pointee->NoticeBodyUncontrolled();
            }
            result = this->timeUncontrolled + dt;
            float tf = this->timeFalling;
            this->timeUncontrolled = this->timeUncontrolled + dt;
            if (result <= tf)
                result = tf;
        }
    }
    else
    {
        st->body.val &= ~0x4000000;
        result = AI_BODY_UNCONTROLLED_TIME_DISABLED;
        if (this->timeUncontrolled > AI_BODY_UNCONTROLLED_TIME_DISABLED)
        {
            this->pBrain->mind.best.pointee->NoticeBodyControllable();
            result = AI_BODY_UNCONTROLLED_TIME_DISABLED;
        }
    }
    this->timeUncontrolled = result;
}
