#pragma once
// ws-engine package: the pak/package manager's load state machine (package::PKG_MNG::state).
// DB-verified values (types_enum_values package::PKG_MNG_STATES, 11 enumerators, value_index 0..10;
// types.size = 4). Drives the fill-list -> open -> close sequence PKG_MNG_XENON runs against the
// Xbox content-package API; the *_ERROR states are the terminal failures of open/close.

namespace package {

enum PKG_MNG_STATES : int {
    pmsIDLE              = 0,
    pmsFILL_LIST_STARTED = 1,
    pmsFILL_LIST         = 2,
    pmsFILL_LIST_STOPPED = 3,
    pmsWAIT_FOR_OPEN     = 4,
    pmsOPENED            = 5,
    pmsOPEN_ERROR        = 6,
    pmsWAIT_FOR_CLOSE    = 7,
    pmsCLOSED            = 8,
    pmsCLOSE_ERROR       = 9,
    pmsNEED_REFRESH      = 10,
};

} // namespace package
