#include "../../headers/hcex/ssl_exporter/Handler.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void, dsTSTRING<char>, int, bool, bool>::Handler(
//     void (haloENGINE_CONTROL::*_pf)(dsTSTRING<char>, int, bool, bool)) @ 0x823C2CF0
//
// Ctor binding a haloENGINE_CONTROL method of shape void(dsTSTRING<char>, int, bool, bool) — e.g.
// the LoadLevelCoop(map, difficulty, isNew, ...) SSL export. Body is the inline member-template
// ctor on ds_data_call::Handler (Handler.h): store pf.
