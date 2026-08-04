#include "sslSCRIPTED_FUNCTION_BASE.h"
#include "sslFUNCTION_DECL.h"
#include "sslSTACK.h"
#include "sslSTACK_DATA.h"
#include "sslOBJECT.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsDATA_MATH.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary
// Table of binary-operator display names indexed by dsDATA_BINARY_OP; not reversed here.
extern const char *dsBinaryOpName[]; // boundary

// 0x82AD8470 — sslSCRIPTED_FUNCTION_BASE::MathFromStackToStack. Pop the top two operands off the VM
// operand stack, apply binary operator `op` (left op right), and push the result. Returns sslERROR by
// value (sret): the decompiler swaps this/result (r3 = return slot sslERROR*, r4 = this
// sslSCRIPTED_FUNCTION_BASE*) and mistypes pSelf as dsTSTRING<char>*. Protected (IAA).
sslERROR sslSCRIPTED_FUNCTION_BASE::MathFromStackToStack(sslOBJECT *pSelf, sslSTACK &stack, dsDATA_BINARY_OP op)
{
    dsDATA rightData;  rightData.type  = nullptr;
    dsDATA leftData;   leftData.type   = nullptr;
    dsDATA resultData; resultData.type = nullptr;

    // Fetch and pop the right-hand operand (top of stack).
    sslERROR err = stack.GetTop().GetAsDsData(rightData);
    stack.Pop();
    if (err.id)
    {
        if (rightData.type)
            rightData.type->Destroy(&rightData.storage);
        return err;
    }

    // Fetch and pop the left-hand operand (now on top).
    err = stack.GetTop().GetAsDsData(leftData);
    stack.Pop();

    sslERROR result;
    if (err.id)
    {
        result = err;
    }
    else if (leftData.Math(op, rightData, resultData) == DSD_MATH_OK)
    {
        stack.Push().SetToDsData(resultData);
        result = err; // no-error
    }
    else
    {
        dsTSTRING<char> objName = pSelf->GetDbgName();
        const char *leftName  = leftData.type  ? leftData.type->GetDbgName()  : "null";
        const char *rightName = rightData.type ? rightData.type->GetDbgName() : "null";
        const char *funcName  = this->pDecl->name.CStr();
        dsTSTRING<char> msg;
        result = sslERROR(SSL_ERR_GENERIC,
                          *dsSPrintf(&msg, "can't perform math operation '%s' '%s' '%s' in '$%s.%s'",
                                     dsBinaryOpName[op], leftName, rightName, objName.CStr(), funcName),
                          0);
    }

    if (resultData.type)
        resultData.type->Destroy(&resultData.storage);
    if (rightData.type)
        rightData.type->Destroy(&rightData.storage);
    if (leftData.type)
        leftData.type->Destroy(&leftData.storage);
    return result;
}
