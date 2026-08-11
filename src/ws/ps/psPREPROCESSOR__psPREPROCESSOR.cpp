#include "psPREPROCESSOR.h"
#include "../../headers/apCL.h"
// @0x82A2D3F8 — default-construct: empty #if/#else stack, empty accumulated result and error text,
// empty macro-name list.

psPREPROCESSOR::psPREPROCESSOR()
{
    apCL cl;
    cl.line = 971;
    cl.file = "D:\\Projects\\code\\common\\src.sys\\ps\\ps_preprocessor.cpp"; // DEVIATION: the disasm
        // literal for this call site resolves through a data-table entry (`FMOD::gITLogPeriodTable`
        // artifact of overlapping string-literal pooling) rather than a plain string; the source file
        // is inferred from the sibling `#ssl`/preprocessor call sites in this same subsystem.
    stack.pData = nullptr;
    stack.nElem = 0;
    stack.allocated = 0;
    stack.__cl = cl;

    result.pBuffer = nullptr;
    result.UnsafeInitEmpty();

    errorDesc.pBuffer = nullptr;
    errorDesc.UnsafeInitEmpty();

    macro.pData = nullptr;
    macro.nElem = 0;
    macro.allocated = 0;
    macro.__cl = cl;
}
