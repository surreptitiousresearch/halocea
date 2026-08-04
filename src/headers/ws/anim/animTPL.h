#pragma once
// Canonical animTPL definition lives in the flat header src/headers/animTPL.h (picked up by every
// TU via `#include "animTPL.h"`). Forward to it so a TU reaching this organized-tree path (scn
// headers use "../anim/animTPL.h") does not get a duplicate typedef (C2011). Layout is identical.
#include "../../animTPL.h"
