#include "../../headers/hcex/ssl_exporter/Handler.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void, dsTSTRING<char>, int, bool>::Handler(
//     void (haloENGINE_CONTROL::*_pf)(dsTSTRING<char>, int, bool)) @ 0x823C2AF0
//
// Ctor binding a haloENGINE_CONTROL method of shape void(dsTSTRING<char>, int, bool) — e.g. the
// LoadLevelSolo(map, difficulty, isNew) SSL export. Body is the inline member-template ctor on
// ds_data_call::Handler (Handler.h): store pf (the base+derived vtable writes the decompiler shows
// are implicit compiler codegen, not source).
