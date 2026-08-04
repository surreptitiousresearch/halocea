#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslCLASS_REF.h"
#include "../../headers/ws/ssl/sslCLASS.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ssl/sslERROR.h" // GetVar returns sslERROR by value (discarded)

// aiCON_CB::GetAISSLVarsBrain @ 0x83181B00
// Enumerate the selected watchee brain's script-class variable sub-elements and build a
// separator(\x01)-delimited "name=dbgvalue" list into `list`.
void aiCON_CB::GetAISSLVarsBrain(dsTSTRING<char> &list)
{
    list = dsTSTRING<char>(); // start empty

    ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = this->pCurrentWatchee.pHandle;
    aiBRAIN *brain = handle ? handle->pPtr : nullptr;

    // The brain's own script self-reference. DB does not type aiBRAIN+0x58; reproduced as the
    // raw offset the binary uses (disasm: pPtr[+22] as sslCLASS_REF*, i.e. +0x58 bytes). boundary offset.
    sslOBJ_REF *brainObj = reinterpret_cast<sslOBJ_REF *>(reinterpret_cast<char *>(brain) + 0x58);

    dsDATA value;
    value.type = nullptr; // reused across iterations; GetVar drops the prior value each time

    sslCLASS_REF classRef = brainObj->GetClass();
    int count = classRef.pClass ? classRef.pClass->elements.nElem : 0;

    for (int i = 0; i < count; ++i)
    {
        sslCLASS_REF elemClass = brainObj->GetClass();
        if (!elemClass.pClass->IsSubElemVar(i))
            continue;

        // separator between entries
        if (list.pBuffer->strLen)
            list.Insert(list.pBuffer->strLen, (char)1, 1);

        dsSTRID name = brainObj->GetClass().pClass->GetSubElemName(i);
        dsTSTRING<char> line(name.CStr());

        brainObj->GetVar(i, value);              // sslERROR result discarded
        dsTSTRING<char> valueStr = value.GetDbgString();

        line.Insert(line.pBuffer->strLen, "=", -1);
        line.Insert(line.pBuffer->strLen, valueStr);

        list.Insert(list.pBuffer->strLen, line);
    }
}
