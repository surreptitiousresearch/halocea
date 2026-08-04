#ifndef STANDARD_REPEAT_DELAY_H
#define STANDARD_REPEAT_DELAY_H

/* STANDARD_REPEAT_DELAY_MILLISECONDS — UI widget activation/repeat hold delay.
 * DB-verified via types_enum_values $10AE7F4258F1E889A20AC3A809E6036A (extracted from that
 * merged anonymous UI-constants enum; the blob's other members are unrelated sprite-frame
 * indices kept out of this header). */
enum standard_repeat_delay {
	STANDARD_REPEAT_DELAY_MILLISECONDS = 250,
};

#endif /* STANDARD_REPEAT_DELAY_H */
