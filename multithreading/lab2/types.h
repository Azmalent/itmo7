#ifndef LAB2_TYPES
#define LAB2_TYPES

#define TRUE  1
#define FALSE 0

typedef int bool;

typedef enum {
    RECURSIVE = 1,
    NO_ERRORS = 2,
    VERBOSE = 4,
    CHANGES_ONLY = 8
} flags_t;

typedef enum {
    OWNER = 1,
    GROUP = 2,
    OTHERS = 4,
    DEFAULT = OWNER | GROUP | 8
} class_t;

typedef enum {
    GRANT_PERMS = 1,
    REVOKE_PERMS = 2,
    SET_PERMS = 3
} operation_t;

typedef enum {
    EXECUTE = 1,
    WRITE = 2,
    READ = 4
} perms_t;

#endif