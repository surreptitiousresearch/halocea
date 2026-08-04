#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */

/* XC_LANGUAGE_* — Xbox system language codes returned by XGetLanguage (imported here as XTLGetLanguage).
 * SDK-sourced: values from the Xbox/Xbox 360 XDK <xconfig.h> XC_LANGUAGE_* constants; not an engine enum. */
enum xc_language {
	XC_LANGUAGE_ENGLISH = 1,
	XC_LANGUAGE_JAPANESE = 2,
	XC_LANGUAGE_GERMAN = 3,
	XC_LANGUAGE_FRENCH = 4,
	XC_LANGUAGE_SPANISH = 5,
	XC_LANGUAGE_ITALIAN = 6,
	XC_LANGUAGE_KOREAN = 7,
	XC_LANGUAGE_TCHINESE = 8,
	XC_LANGUAGE_PORTUGUESE = 9,
};
