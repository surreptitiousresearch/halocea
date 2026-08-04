#pragma once
/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _773F498A446289C99DCA681DC880F61E (DB _audio_feedback_*; value 3 = _audio_feedback_backward,
 * 4 = _audio_feedback_failure — the local "back"/"flag_failure" labels are the consumer names, kept). */

/* GUESS: enum recovered by reconciliation, no ground-truth source for the enum NAME — but the value→meaning
 * mapping is binary ground truth: both ui_play_audio_feedback_sound @0x83733210 and
 * widget_instance_process_one_event_recursive @0x837384B0 map 1 -> "sound\sfx\ui\cursor",
 * 2 -> "sound\sfx\ui\forward", 3 -> "sound\sfx\ui\back", 4 (default of the 1..4 range) ->
 * "sound\sfx\ui\flag_failure". Member names are taken verbatim from those sound tag paths. */
enum ui_audio_feedback_sound {
	_ui_audio_feedback_none = 0,
	_ui_audio_feedback_cursor = 1,
	_ui_audio_feedback_forward = 2,
	_ui_audio_feedback_back = 3,
	_ui_audio_feedback_flag_failure = 4,
};
