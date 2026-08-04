#pragma once
/* ai_information_look_type — the "look_type" arm of ai_information_packet (selects the
 * look_data union: none / unit / object). Stored into ai_information_packet.look_type.
 * Source: DB types_enum_values _76E79690B59A3B3F72ECE1FCB3338F13. */

enum ai_information_look_type {
	_ai_information_look_none = 0,
	_ai_information_look_unit = 1,
	_ai_information_look_object = 2,
};

// ai_information_look_type: DB-verified via types_enum_values _76E79690B59A3B3F72ECE1FCB3338F13 (anonymous compiled enum)
