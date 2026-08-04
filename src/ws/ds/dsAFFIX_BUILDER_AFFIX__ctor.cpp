#include "dsAFFIX_BUILDER.h"

// dsAFFIX_BUILDER::AFFIX::AFFIX() @ 0x82A75FD0
// Default-construct a blank affix: point the name at the process-wide shared empty-string
// singleton (bumping its refcount) and start with an empty parameter vector tagged with the
// ds_affixes.h:200 call-site cookie.
dsAFFIX_BUILDER::AFFIX::AFFIX()
{
    this->name.pBuffer = nullptr;
    // dsTSTRING<char>::UnsafeInitEmpty was inlined here (lazy singleton init + refcount bump).
    // The decompiler misread the "++refCount" as a store of the cookie string; the disasm
    // (lwz/addi 1/stw at 0x82A7604C..0x82A76068) confirms it is an increment.
    this->name.UnsafeInitEmpty();

    this->params.pData = nullptr;
    this->params.nElem = 0;
    this->params.allocated = 0;
    this->params.__cl.file = "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_affixes.h";
    this->params.__cl.line = 200;
}
