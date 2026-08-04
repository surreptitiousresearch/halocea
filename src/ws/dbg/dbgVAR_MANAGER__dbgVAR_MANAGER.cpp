#include "dbgVAR_MANAGER.h"

// dbgVAR_MANAGER::dbgVAR_MANAGER() @ 0x82560e00
// Default constructor for the process-wide debug-variable registry: default-constructs the
// CONSOLE_CALLBACK base and the `overrides` map (both tagged with this constructor's own
// file/line as their allocation call-site cookie), then hand-initialises vars/newVars/lock/
// needUpdateConsole in the body.
// NOTE: the decompiler produced two separate apCL locals (v2/v3) and showed newVars.__cl.file as
// `&dbgVAR_MANAGER::`vftable'` -- the disasm shows this is a decompiler mis-read: both __cl values
// are reloaded from the SAME stack scratch slot holding {file="D:\\Projects\\code\\common\\src.sys\\
// ap\\ap_dbg_var.cpp", line=25}, so vars.__cl and newVars.__cl are identical. Reconstructed with one
// shared literal used for both, and for the `overrides` call-site cookie.
dbgVAR_MANAGER::dbgVAR_MANAGER()
    : con::CONSOLE_CALLBACK()
    , overrides(apCL{ "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dbg_var.cpp", 25 })
{
    this->vars.pData     = nullptr;
    this->vars.nElem     = 0;
    this->vars.allocated = 0;
    this->vars.__cl      = apCL{ "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dbg_var.cpp", 25 };

    this->newVars.pData     = nullptr;
    this->newVars.nElem     = 0;
    this->newVars.allocated = 0;
    this->newVars.__cl      = apCL{ "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dbg_var.cpp", 25 };

    this->needUpdateConsole = false;

    this->lock.isInited    = false;
    this->lock.threadNmb   = -1;
    this->lock.locker_file = nullptr;
    this->lock.locker_line = -1;
    this->lock.name        = "Unknown";
    this->lock.Init();
}
