#include "../../headers/hcex/ssl_exporter/Handler.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, bool>::Handler(
//     bool (haloENGINE_CONTROL::*_pf)()) @ 0x823C2F98
//
// Ctor binding a nullary bool haloENGINE_CONTROL method — e.g. IsCoop(). Body is the inline
// member-template ctor on ds_data_call::Handler (Handler.h): store pf. The typedef forces this
// instantiation through the compile gate.
typedef ds_data_call::Handler<0, haloENGINE_CONTROL, bool> Handler_bool;
