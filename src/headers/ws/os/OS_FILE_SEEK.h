#pragma once
// ws-engine os: seek-origin enum for osFileSeek. DB-verified (types_enum_values OS_FILE_SEEK):
// exactly 3 values -- no 4th "from end of a larger container" style value exists in the DB.
enum OS_FILE_SEEK {
    OS_FILE_SEEK_SET = 0,
    OS_FILE_SEEK_CUR = 1,
    OS_FILE_SEEK_END = 2,
};
