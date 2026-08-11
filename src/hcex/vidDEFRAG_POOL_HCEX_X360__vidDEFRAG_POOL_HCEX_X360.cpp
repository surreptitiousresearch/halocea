#include "../headers/ws/vid/vidDEFRAG_POOL_HCEX_X360.h"

// vidDEFRAG_POOL_HCEX_X360::vidDEFRAG_POOL_HCEX_X360 @ 0x823F1348
// Zero the `buffers` and `descs` dsVECTORs and install this class's vtable. Both vectors adopt the
// same allocation call-site cookie {file="...ap_defrag_mng.h", line=123}.
// DEVIATION: the decompiler mis-rendered `buffers.__cl.file` as a cast of the class's own vtable
// pointer (register-reuse artifact — r6, holding the vtable address, is loaded right after the
// two apCL stack temporaries). Verified against disasm at 0x823F1354: both `buffers.__cl` and
// `descs.__cl` are populated from the same reloaded 8-byte {file,line} stack value (std r5/r7,
// both sourced from var_10(r1)), so `buffers.__cl.file` is the same string literal as
// `descs.__cl.file`, not the vtable pointer.
vidDEFRAG_POOL_HCEX_X360::vidDEFRAG_POOL_HCEX_X360()
{
    this->buffers.pData = nullptr;
    this->buffers.nElem = 0;
    this->buffers.allocated = 0;
    this->buffers.__cl.file = "D:\\Projects\\code\\common\\INCL.SYS\\ap\\ap_defrag_mng.h";
    this->buffers.__cl.line = 123;

    this->descs.pData = nullptr;
    this->descs.nElem = 0;
    this->descs.allocated = 0;
    this->descs.__cl.file = "D:\\Projects\\code\\common\\INCL.SYS\\ap\\ap_defrag_mng.h";
    this->descs.__cl.line = 123;

    this->__vftable = (apDEFRAG_POOL_vtbl *)&vidDEFRAG_POOL_HCEX_X360::vftable; // base __vftable type
}
