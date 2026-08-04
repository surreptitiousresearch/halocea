#ifndef CACHE_COPY_STATUS_H
#define CACHE_COPY_STATUS_H

/* Low-level background map-copy status returned by cache_copy_get_status.
 * DB anonymous enum _B86638382CB2580EAD656C680E290F89 (value_name spellings verbatim). */
typedef enum cache_copy_status
{
    _cache_copy_bad_file_failure = 0,
    _cache_copy_read_failure     = 1,
    _cache_copy_write_failure    = 2,
    _cache_copy_in_progress      = 3,
    _cache_copy_finised          = 4,
    NUMBER_OF_CACHE_COPY_STATES  = 5
} cache_copy_status;

#endif /* CACHE_COPY_STATUS_H */
