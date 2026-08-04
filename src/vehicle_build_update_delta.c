/* vehicle_build_update_delta @0x8375E0E8 — encode a network update-delta for a vehicle object. Sibling of
 * biped_build_update_delta.c, sharing the same header/payload/encode shape, with two vehicle-specific
 * differences: (1) an early-out when the "new vehicle update scheme" is enabled and the vehicle's driver
 * seat is filled and the caller wants an incremental (not stateless) update — that case is handled by the
 * new scheme elsewhere; (2) the incremental payload is the vehicle's own position/translational_velocity/
 * angular_velocity/forward/up (headers/object_datum.h — the raw offsets matched that header's fields
 * exactly once the pointer's leading definition_index dword was accounted for), rather than biped's small
 * per-object payload.
 *
 * DEVIATION: `_cntlzw(mode) & 0x20` is the "mode == 0" idiom (see biped_build_update_delta.c), simplified to
 * `mode == _message_delta_mode_stateless` / the initial-guard's `mode` truthy check to
 * `mode == _message_delta_mode_incremental`. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/message_delta_message_ids.h"
#include "headers/object_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/object_flags.h"
#include "headers/vehicle_update_header.h"
#include "headers/vehicle_update_payload.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t unit_driver_seat_is_filled(int unit_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern uint32_t system_milliseconds(void);
extern message_delta_processor_message_definition_type object_type_get_update_message_type(int16_t object_type);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

int vehicle_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits,
    const message_delta_processor_mode mode)
{
    if (unit_driver_seat_is_filled(object_index) && mode == _message_delta_mode_incremental
            && use_new_vehicle_update_scheme)
        return 0;

    /* typed as the full vehicle record (raw offsets 1316-1320 are _vehicle_datum @1228 members) */
    vehicle_datum *vehicle = object_try_and_get_and_verify_type(object_index, object_mask_vehicle);
    if (!vehicle)
        return 0;

    _vehicle_update_header header;
    header.translated_object_index =
            field_translated_index_translate_index(&field_properties_object_index_definition, object_index);
    header.sequence_number = vehicle->vehicle.message_index;
    header.baseline_index = vehicle->vehicle.baseline_index;
    header.mode_is_stateless = (mode == _message_delta_mode_stateless);

    header.timestamp = system_milliseconds();
    message_delta_processor_message_definition_type update_message_type =
            object_type_get_update_message_type(vehicle->object.type);

    int result;
    if (mode == _message_delta_mode_incremental)
    {
        _vehicle_update_payload payload;
        payload.at_rest_bit = (unsigned char)((vehicle->object.flags >> _object_at_rest_bit) & 1);
        payload.position = vehicle->object.position;
        payload.translational_velocity = vehicle->object.translational_velocity;
        payload.angular_velocity = vehicle->object.angular_velocity;
        payload.forward = vehicle->object.forward;
        payload.up = vehicle->object.up;

        result = message_delta_processor_encode_incremental(update_message_type, &header, &payload,
                &vehicle->vehicle.baseline, buffer, buffer_size_in_bits, 0);
    }
    else
    {
        result = message_delta_processor_encode_stateless(update_message_type, &header, &vehicle->vehicle.baseline,
                buffer, buffer_size_in_bits);
    }

    vehicle->vehicle.has_been_accelerated_since_last_incremental = 0;
    if (result > 0)
    {
        unsigned int next_sequence_number = (unsigned char)(vehicle->vehicle.message_index + 1);
        vehicle->vehicle.message_index = (unsigned __int8)next_sequence_number;
        if (next_sequence_number >= 0xFF)
            vehicle->vehicle.message_index = 0;
    }
    vehicle->unit.force_local_update = 0;

    return result;
}
