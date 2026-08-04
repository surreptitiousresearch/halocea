#ifndef ANALOG_STICK_EVENT_CONSTANTS_H
#define ANALOG_STICK_EVENT_CONSTANTS_H

/* analog_stick_event_constants — UI input-event pacing constants used by input_mode_handle_events
 * (the same 350 ms throttle also paces the d-pad repeat in that function).
 * DB-verified via types_enum_values $5E53499F2A45333BDC637ABD458D1D0A. */
enum analog_stick_event_constants {
	MILLISECOND_DELAY_BETWEEN_SUBSEQUENT_ANALOG_STICK_EVENTS = 350,
	ANALOG_STICK_RECOGNITION_THRESHOLD = 29490,
};

#endif /* ANALOG_STICK_EVENT_CONSTANTS_H */
