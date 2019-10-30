#ifndef LAB2_TYPES
#define LAB2_TYPES

typedef enum {
    RECURSIVE = 1,
    NO_ERRORS = 2,
    VERBOSE = 4,
    CHANGES_ONLY = 8
} flags_t;

typedef enum {
    DEFAULT = 0,
    OWNER = 1,
    GROUP = 2,
    OTHERS = 4,
    EVERYONE = OWNER | GROUP | OTHERS
} class_t;

typedef enum {
    GRANT_PERMS = 0,
    REVOKE_PERMS = 1,
    SET_PERMS = 2
} operation_t;

typedef enum {
    EXECUTE = 1,
    WRITE = 2,
    READ = 4,
    SPECIAL_EXECUTE = 8,
    SET_ID = 16,
    STICKY = 32
} perms_t;

#endif