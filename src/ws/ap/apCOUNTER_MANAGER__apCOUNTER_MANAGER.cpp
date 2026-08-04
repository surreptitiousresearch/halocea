#include "apCOUNTER_MANAGER.h"

// apCOUNTER_MANAGER::apCOUNTER_MANAGER @ 0x82715E00 — private (AAA mangle), only called from
// Instance().
//
// CAVEAT: the raw decompile aliases three apCL source-location cookies (for counters,
// countersToSend, profilers) through overlapping stack temporaries, and one of them briefly
// shows `file` pointing at the apCOUNTER_MANAGER vtable -- a register-reuse artifact, not real
// code (a source file pointer can't legitimately be the vtable address). Reconstructed here as
// three independent apCL cookies, all citing ap_prof_counters.cpp with the line numbers the
// decompile's non-garbage assignments actually carried (322 for counters, 323 for the other two).
apCOUNTER_MANAGER::apCOUNTER_MANAGER()
    : con::CONSOLE_CALLBACK()
{
    this->__vftable = (con::CONSOLE_CALLBACK_vtbl *)&apCOUNTER_MANAGER::vftable;

    this->counters.pData = nullptr;
    this->counters.nElem = 0;
    this->counters.allocated = 0;
    this->counters.__cl.file = "D:\\Projects\\code\\common\\src.sys\\ap\\ap_prof_counters.cpp";
    this->counters.__cl.line = 322;

    this->isChanged = false;

    this->countersToSend.pData = nullptr;
    this->countersToSend.nElem = 0;
    this->countersToSend.allocated = 0;
    this->countersToSend.__cl.file = "D:\\Projects\\code\\common\\src.sys\\ap\\ap_prof_counters.cpp";
    this->countersToSend.__cl.line = 323;

    this->isFirstUpdate = false;
    this->commonSettings.state = 0;

    this->profilers.pData = nullptr;
    this->profilers.nElem = 0;
    this->profilers.allocated = 0;
    this->profilers.__cl.file = "D:\\Projects\\code\\common\\src.sys\\ap\\ap_prof_counters.cpp";
    this->profilers.__cl.line = 323;

    // this->cntTracker is default-constructed implicitly (apCOUNTER_TRACKER's own ctor runs here).

    this->lock.isInited = false;
    this->lock.locker_file = nullptr;
    this->lock.threadNmb = -1;
    this->lock.name = "Unknown";
    this->lock.locker_line = -1;
    this->lock.Init();

    this->cameraManager = nullptr;
}
