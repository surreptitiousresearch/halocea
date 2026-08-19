/* ?SetCurDip@vidDIP_STAT@@QAAXPBD0K@Z @0x8266D108 */
#include "headers/ws/vid/vidDIP.h"
#include "headers/ws/ds/dsCMP.h"
#include <string.h>

// 0x8266D114 -- reversed against the mangled signature (?SetCurDip@vidDIP_STAT@@QAAXPBD0K@Z ==
// (const char*, const char*, unsigned long)). DEVIATION: the decompiler's raw output shows this
// taking 16 params (a5..a16) -- a known stack-slot-reuse artifact from PPC register-window
// spilling, the same pattern already documented in psSYSTEM.h; only the first 3 are real.
//
// Guarded by `lock`; no-op unless this stat block is active and a group has already been
// selected via SetCurGroup. Lazily creates (find-or-insert, by name) the current class/dip
// records under curGroup/curClass, and the current dip-id record (by numeric id) under curDip,
// resetting the more-specific selections whenever a less-specific one changes.
void vidDIP_STAT::SetCurDip(const char *nameClass, const char *nameDip, unsigned long dipID)
{
    if (!this->isActive || !this->curGroup)
        return;

    this->lock.Lock(nullptr, 0);

    if (!this->curClass || strcmp(this->curClass->name.CStr(), nameClass) != 0)
    {
        dsNAME_CMP<vidDIP_CLASS, 1, 1> nameCmp;
        int idx = this->curGroup->dipClassList.Find<dsNAME_CMP<vidDIP_CLASS, 1, 1>, const char *>(nameClass, nameCmp, 0);
        if (idx == -1)
        {
            vidDIP_CLASS newClass;
            newClass.name = nameClass;
            this->curClass = &this->curGroup->dipClassList.PushBack(newClass);
        }
        else
        {
            this->curClass = &this->curGroup->dipClassList[idx];
        }
        this->curDip = nullptr;
        this->curDipID = nullptr;
    }

    if (!this->curDip || strcmp(this->curDip->name.CStr(), nameDip) != 0)
    {
        dsNAME_CMP<vidDIP, 1, 1> nameCmp;
        int idx = this->curClass->dipList.Find<dsNAME_CMP<vidDIP, 1, 1>, const char *>(nameDip, nameCmp, 0);
        if (idx == -1)
        {
            vidDIP newDip;
            newDip.name = nameDip;
            this->curDip = &this->curClass->dipList.PushBack(newDip);
        }
        else
        {
            this->curDip = &this->curClass->dipList[idx];
        }
        this->curDipID = nullptr;
    }

    if (!this->curDipID || this->curDipID->id != dipID)
    {
        vidDIP_ID key;
        key.id = dipID;
        key.counter.dipCur = 0;
        key.counter.dipMax = 0;
        key.counter.polyCur = 0;

        dsCMP idCmp;
        int idx = this->curDip->dipIDList.Find<dsCMP, vidDIP_ID>(key, idCmp, 0);
        if (idx == -1)
        {
            this->curDipID = &this->curDip->dipIDList.PushBack(key);
        }
        else
        {
            this->curDipID = &this->curDip->dipIDList[idx];
        }
    }

    this->lock.Unlock(nullptr, 0);
}
