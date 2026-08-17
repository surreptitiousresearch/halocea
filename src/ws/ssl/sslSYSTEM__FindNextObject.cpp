/* ?FindNextObject@sslSYSTEM@@QAA?AVsslOBJ_REF@@AAHVsslCLASS_REF@@@Z @0x825CECD8 */
#include "sslSYSTEM.h"
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"
#include "sslCLASS_REF.h"

// 0x825CECD8 — sslSYSTEM::FindNextObject. Returns sslOBJ_REF by value (sret): the decompiler swaps
// this/result (r3 = return slot sslOBJ_REF*, r4 = this sslSYSTEM*). Advance `curIdx` to the next
// live object that is (a subclass of) `cls`, skipping objects still under construction.
// `cls` is passed by value (its class ref is released on return).
sslOBJ_REF sslSYSTEM::FindNextObject(int &curIdx, sslCLASS_REF cls)
{
    ++curIdx;
    if (curIdx >= this->allObjects.nElem)
        return sslOBJ_REF();

    while (true)
    {
        if ((this->allObjects[curIdx].pObject->state.state & 1) == 0)   // not still constructing
        {
            bool match;
            if (!cls.pClass)
            {
                match = true;   // no filter: any live object qualifies
            }
            else
            {
                sslCLASS_REF objClass = this->allObjects[curIdx].GetClass();
                match = objClass.pClass->IsSubClassOf(cls.pClass) != 0;
            }
            if (match)
                break;
        }

        ++curIdx;
        if (curIdx >= this->allObjects.nElem)
            return sslOBJ_REF();
    }

    return this->allObjects[curIdx];
}
