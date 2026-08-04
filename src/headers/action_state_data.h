#pragma once
/* action_state_data — the per-action working-state block embedded in actor_state_data at +0x3C (132 bytes). It
 * is an anonymous union of the per-action-class state payloads; only the arm matching the actor's active action
 * class (actor->state.action) is meaningful. Layout is types_members-confirmed: the union spans 132 bytes,
 * sized by its largest arm (obey_state_data). */

#include "sleep_state_data.h"
#include "alert_state_data.h"
#include "fight_state_data.h"
#include "guard_state_data.h"
#include "flee_state_data.h"
#include "uncover_state_data.h"
#include "search_state_data.h"
#include "wait_state_data.h"
#include "vehicle_state_data.h"
#include "charge_state_data.h"
#include "obey_state_data.h"
#include "converse_state_data.h"
#include "avoid_state_data.h"

/* DB anonymous union _A782C58677239A2F18B7077D74B7DB52 (132 bytes) - action_state_data member
 * ___u0. $-name kept verbatim ($ in identifiers is an MSVC extension), matching the mtlPARAM.h
 * convention. */
typedef union _A782C58677239A2F18B7077D74B7DB52
{
        sleep_state_data    sleep;      /* 2 bytes  */
        alert_state_data    alert;      /* 92 bytes */
        fight_state_data    fight;      /* 4 bytes  */
        guard_state_data    guard;      /* 68 bytes */
        flee_state_data     flee;       /* 48 bytes */
        uncover_state_data  uncover;    /* 52 bytes */
        search_state_data   search;     /* 44 bytes */
        wait_state_data     wait;       /* 24 bytes */
        vehicle_state_data  vehicle;    /* 76 bytes */
        charge_state_data   charge;     /* 56 bytes */
        obey_state_data     obey;       /* 132 bytes — sizes the union */
        converse_state_data converse;   /* 20 bytes */
        avoid_state_data    avoid;      /* 4 bytes  */
} _A782C58677239A2F18B7077D74B7DB52;                                      /* 132 bytes */

typedef struct action_state_data
{
    _A782C58677239A2F18B7077D74B7DB52 ___u0;   /* 0x00 - DB member name for the anonymous union */
} action_state_data;                    /* 132 bytes */
