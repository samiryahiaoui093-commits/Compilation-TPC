#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef char* string;

typedef enum {
    FUNCTION, GLOBAL_VARIABLE, LOCAL_VARIABLE, STRUCTURE_INSTANCE, STRUCTURE, VARIABLE
} Tables;

typedef struct {
    string name;
    string function_name;
    Tables table;
    string type;
    union {
        char char_value;
        int int_value;
    };
    int definition_line;
} Variable;

typedef struct {
    string name;
    Tables table;
    string type;
    string *argument_type;
    string *argument_names;
    int argument_count;
    int definition_line;
} Function;

typedef struct Structure {
    string name;
    Tables table;
    string function_name;
    string *argument_type;
    string *argument_names;
    int definition_line;
} Structure;

typedef struct Chained_Node {
    string key;
    Tables tag;
    union {
        Variable *variable;
        Structure* structure;
        Function* function;
    };
    struct Chained_Node *next;
} Chained_Node;

/* Tables globales */
extern Chained_Node *local_variable[50];
extern Chained_Node *global_variable[50];
extern Chained_Node *functions_definitions[50];
extern Chained_Node *structure_definitions[50];

/* Fonctions AST */


/* Symbol table */
void parse_tree(Node* root);
void printFunction(Function *f);

/* Création */
Function *create_function(Node* node);
Variable *create_variable(Node* node, string function_name);
Structure *create_structure(Node * node, string function_name);

void print_chain(Chained_Node *node);
void print_symbol_tables();

#endif
