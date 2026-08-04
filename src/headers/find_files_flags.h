#ifndef FIND_FILES_FLAGS_H
#define FIND_FILES_FLAGS_H

/* find_files_flags — find_files flags.
 * DB-verified: types_enum_values _3BB901B9596B139CD611B9F64EADD532 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum find_files_flags {
	_find_files_recursive_bit = 0,
	_find_files_enumerate_directories_bit = 1,
	NUMBER_OF_FIND_FILES_FLAGS = 2,
};

#endif /* FIND_FILES_FLAGS_H */

// find_files_flags: DB-verified via types_enum_values _3BB901B9596B139CD611B9F64EADD532
