#include "../../headers/hcex/ssl_exporter/Handler.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, ds_data::REF_TYPE<dsREF_VECTOR>,
//     dsTSTRING<char> const &>::Handler(
//     ds_data::REF_TYPE<dsREF_VECTOR> (haloENGINE_CONTROL::*_pf)(const dsTSTRING<char> &))
//     @ 0x823C2E40
//
// Ctor binding a haloENGINE_CONTROL method returning a ds_data::REF_TYPE<dsREF_VECTOR> (a
// script-visible vector reference) from a const dsTSTRING<char>& argument — e.g.
// CreateCheckpointList(level). ds_data::REF_TYPE<dsREF_VECTOR> and dsREF_VECTOR are ws-engine
// ds_data infrastructure, beyond this batch's 2-level descent — boundary. Body is the inline
// member-template ctor on ds_data_call::Handler (Handler.h): store pf (held in the generic member
// -fn-ptr slot, so the exact return type is not needed to reproduce the ctor).
