#include "../../headers/hcex/ssl_exporter/Handler.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void, bool>::Handler(
//     void (haloENGINE_CONTROL::*_pf)(bool)) @ 0x823C2F40
//
// Ctor binding a haloENGINE_CONTROL method of shape void(bool) — e.g. SetPause(paused). Body is the
// inline member-template ctor on ds_data_call::Handler (Handler.h): store pf. The typedef forces
// this instantiation through the compile gate.
typedef ds_data_call::Handler<0, haloENGINE_CONTROL, void, bool> Handler_void_bool;
