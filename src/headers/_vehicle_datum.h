#ifndef __VEHICLE_DATUM_H_
#define __VEHICLE_DATUM_H_
/* _vehicle_datum — the vehicle-specific runtime datum embedded in a vehicle object (parent
 * vehicle_datum.vehicle / vehicle_state.vehicle_data). Reconciled to the DB-verified layout;
 * previously modelled as an opaque 244-byte block. */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "vehicle_datum_network_data.h"

/* DB anonymous union _A6244B1D18010CC5CA3145AD1D60D056 (2 bytes) - _vehicle_datum member ___u27
 * at 0xE4: the scenario spawn-datum index, viewed as a netgame flag index in netgame modes.
 * $-name kept verbatim ($ in identifiers is an MSVC extension), matching the mtlPARAM.h
 * convention. */
typedef union _A6244B1D18010CC5CA3145AD1D60D056
{
    __int16 vehicle_scenario_datum_index;   /* 0x00 */
    __int16 vehicle_netgame_flag_index;     /* 0x00 */
} _A6244B1D18010CC5CA3145AD1D60D056;

typedef struct _vehicle_datum
{
    unsigned __int16 flags;                                       /* 0x00 */
    __int16 stop_time;                                            /* 0x02 */
    unsigned __int8 airborne_ticks;                               /* 0x04 */
    unsigned __int8 upending_type;                                /* 0x05 */
    unsigned __int8 upending_ticks;                               /* 0x06 */
    unsigned __int8 on_ground_ticks;                              /* 0x07 */
    float speed;                                                  /* 0x08 */
    float slide;                                                  /* 0x0C */
    float turn;                                                   /* 0x10 */
    float wheel;                                                  /* 0x14 */
    float left_tread;                                             /* 0x18 */
    float right_tread;                                            /* 0x1C */
    float hover;                                                  /* 0x20 */
    float thrust;                                                 /* 0x24 */
    unsigned __int8 suspension[8];                                /* 0x28 */
    real_point3d hover_position;                                  /* 0x30 */
    real_vector3d collision_force;                                /* 0x3C */
    real_vector3d collision_torque;                               /* 0x48 */
    unsigned int stuck_mass_point_flags;                          /* 0x54 */
    unsigned __int8 has_been_accelerated_since_last_incremental;  /* 0x58 */
    unsigned __int8 baseline_valid;                               /* 0x59 */
    unsigned __int8 baseline_index;                               /* 0x5A */
    unsigned __int8 message_index;                                /* 0x5B */
    vehicle_datum_network_data baseline;                          /* 0x5C */
    unsigned __int8 last_network_data_valid;                      /* 0x9C */
    unsigned __int8 pad_9D[3];                                    /* 0x9D */
    vehicle_datum_network_data last_network_data;                 /* 0xA0 */
    int last_controlled_time;                                     /* 0xE0 */
    _A6244B1D18010CC5CA3145AD1D60D056 ___u27;                     /* 0xE4 - anonymous union, defined above */
    unsigned __int8 pad_E6[2];                                    /* 0xE6 */
    real_point3d spawn_position;                                  /* 0xE8 */
} _vehicle_datum;                                                 /* 244 bytes */

#endif /* __VEHICLE_DATUM_H_ */
