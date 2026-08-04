#ifndef LANGUAGE_CODE_H
#define LANGUAGE_CODE_H

/* Text language / codepage family of the running build (global_language_code):
 * selects the multi-byte lead/trail byte ranges in double_byte_character(). */
enum language_code {
	_language_roman = 0,
	_language_japanese = 1,            /* Shift-JIS */
	_language_simple_chinese = 2,      /* GB2312/EUC-CN */
	_language_traditional_chinese = 3, /* Big5 */
	_language_korean_wansung = 4,      /* UHC/Wansung */
	_language_korean_johab = 5,        /* Johab */
	NUMBER_OF_LANGUAGE_CODES = 6,
};

#endif /* LANGUAGE_CODE_H */

// language_code: DB-verified via types_enum_values $FFB1CE9D02A1CB3258CC79AB6D2C8323
