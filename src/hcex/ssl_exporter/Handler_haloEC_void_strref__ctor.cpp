#include "../../headers/hcex/ssl_exporter/Handler.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void, dsTSTRING<char> const &>::Handler(
//     void (haloENGINE_CONTROL::*_pf)(const dsTSTRING<char> &)) @ 0x823C2DE8
//
// Ctor binding a haloENGINE_CONTROL method of shape void(const dsTSTRING<char> &) — e.g.
// LoadCheckpoint(name). Body is the inline member-template ctor on ds_data_call::Handler
// (Handler.h): store pf.
