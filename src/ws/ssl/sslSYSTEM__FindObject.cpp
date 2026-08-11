#include "sslSYSTEM.h"
#include "sslOBJ_REF.h"
#include "sslERROR.h"

// @0x825CE508 — sslSYSTEM::FindObject. Reset lastErr, then linearly scan the object table for the
// object whose uid matches `uid`; return a ref to it (empty on miss). Returns sslOBJ_REF by value
// (sret): the decompiler swaps this/result (r3 = return slot sslOBJ_REF*, r4 = this sslSYSTEM*).
sslOBJ_REF sslSYSTEM::FindObject(int uid)
{
    this->lastErr = sslERROR(); // clear any prior error to no-error

    for (int i = 0; i < this->allObjects.nElem; ++i)
    {
        if (this->allObjects[i].GetUID() == uid)
            return this->allObjects[i];
    }
    return sslOBJ_REF();
}
