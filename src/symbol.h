#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#define HASH_SIZE 50
typedef char * string;

typedef enum {
    FUNCTION,GLOBAL_VARIABLE,LOCAL_VARIABLE,STRUCTURE_INSTANCE,STRUCTURE,
}Tables;

typedef struct {
        string name;
        Tables table;
        union{
            char char_value;
            int int_value;
        };
        int definition_line;
}Variable;

typedef struct {
        string name;
        Tables table;
        string *argument_type;
        int definition_line;
}Function;

typedef struct Structure{
        string name;
        Tables table;
        string *argument_type;
        string *argument_names;
        int definition_line;
}Structure;


typedef struct Chained_Node {
    char key[50];
    Tables tag;
    union {
        Variable variable;
        Structure structure;
        Function function;
    };
    struct Chained_Node *next;
} Chained_Node;
