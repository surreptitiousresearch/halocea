#include "../../headers/hcex/ssl_exporter/Handler.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void>::HandleCall(
//     haloENGINE_CONTROL *pObj, dsDATA *argv, int argc, dsDATA *retVal) @ 0x823C2D88
//
// Dispatch for the nullary-void binding: invokes the bound member function on pObj, ignoring
// argv/argc/retVal. Body is the inline HandleCall on ds_data_call::Handler (Handler.h):
// (pObj->*pf)(). The typedef below forces the (no-arg) Handler instantiation so the compile gate
// type-checks that HandleCall body.
typedef ds_data_call::Handler<0, haloENGINE_CONTROL, void> Handler_void;
