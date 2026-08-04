#include <new>
/* SavesTaskCreatorHCEX @0x823F4CB0 — factory callback that builds a haloSTORAGE_TASK for a
 * save/load request against the given sysUSER profile's CNT_SAVEDATA content slot.
 *
 * DEVIATION: `(sysUSER_STORAGE_TASK::TASK_TYPE)((_cntlzw(isLoad) & 0x20) == 0)` is the compiler's
 * count-leading-zeros idiom for "isLoad != 0" (cntlzw(0)=32, cntlzw(1)=31, so the expression is
 * true exactly when isLoad is nonzero) -- simplified here to the equivalent `isLoad ? LOAD : SAVE`. */

#include "../headers/hcex/haloSTORAGE_TASK.h"

extern void *operator new(size_t size, const char *file, int line);

// ds_data::REF_TYPE<sysUSER>::Release — boundary (see REF_TYPE<T> note in sysUSER_STORAGE_TASK.h);
// called here in free-function form since that header's local REF_TYPE<T> stub has no methods.
extern void ds_data_REF_TYPE_sysUSER_Release(ds_data::REF_TYPE<sysUSER> *self, void *data);

sysUSER_STORAGE_TASK *SavesTaskCreatorHCEX(ds_data::REF_TYPE<sysUSER> u, bool isLoad, bool async)
{
    haloSTORAGE_TASK *task = (haloSTORAGE_TASK *)operator new(
        0xA4u, "D:\\Projects\\code\\HCEX\\sources\\halo_storage_task.cpp", 0x53);

    sysUSER_STORAGE_TASK *result;
    if (task)
    {
        new (task) haloSTORAGE_TASK(u, isLoad ? sysUSER_STORAGE_TASK::LOAD : sysUSER_STORAGE_TASK::SAVE,
                                     CNT_SAVEDATA, async);
        result = task;
    }
    else
    {
        result = nullptr;
    }

    /* Release() takes `u`'s own REF_TYPE_DATA<sysUSER>* as its second arg; the decompiler shows
     * this via a stack-slot alias, not a distinct value (see ds_data::REF_TYPE<T>::Release note). */
    ds_data_REF_TYPE_sysUSER_Release(&u, u.m_pData);

    return result;
}
