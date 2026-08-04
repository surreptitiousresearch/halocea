#include "../../headers/hcex/ssl_exporter/Handler.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void>::Handler(
//     void (haloENGINE_CONTROL::*_pf)()) @ 0x823C2D48
//
// Ctor binding a nullary void haloENGINE_CONTROL method — e.g. RestartLevel/RestartCheckpoint.
// Body is the inline member-template ctor on ds_data_call::Handler (Handler.h): store pf.
// The typedef below forces this (no-arg) instantiation of the Handler class template, exercising
// its layout and member bodies through the compile gate.
typedef ds_data_call::Handler<0, haloENGINE_CONTROL, void> Handler_void;
