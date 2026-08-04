#include "../../headers/hcex/ssl_exporter/Handler.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, dsTSTRING<char> const &>::Handler(
//     const dsTSTRING<char> &(haloENGINE_CONTROL::*_pf)()) @ 0x823C2FF0
//
// Ctor binding a nullary haloENGINE_CONTROL method returning a const dsTSTRING<char>& — e.g.
// GetCurLevel()/GetCurCheckpoint(). Body is the inline member-template ctor on
// ds_data_call::Handler (Handler.h): store pf.
