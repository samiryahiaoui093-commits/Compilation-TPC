#ifndef SYMBOL_TABLE_UTILS_H
#define SYMBOL_TABLE_UTILS_H

#include "ast.h"

/* =========================
   TYPES
========================= */

typedef char* string;

typedef enum {
    FUNCTION,
    GLOBAL_VARIABLE,
    LOCAL_VARIABLE,
    STRUCTURE_INSTANCE,
    STRUCTURE,
    VARIABLE
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

/* =========================
   GLOBAL TABLES
========================= */

extern Chained_Node *local_variable[50];
extern Chained_Node *global_variable[50];
extern Chained_Node *functions_definitions[50];
extern Chained_Node *structure_definitions[50];

/* =========================
   UTILITIES
========================= */

int len_CL(Node* node);

/* =========================
   ARGUMENTS
========================= */

void transfer_arguments(Node *list, string arguments_type[], string arguments_name[]);

/* =========================
   STRUCT FIELDS
========================= */

int count_structure_field(Node *field_list);
void transfer_fields(Node *list, string arguments_type[], string arguments_name[]);

/* =========================
   FUNCTION CREATION
========================= */

Function *create_function(Node* node);

/* =========================
   VARIABLE CREATION
========================= */

Variable *create_variable(Node* node, string function_name);
Variable *variable_from_parameters(string name, string function_name, int declaration_line, string type);

/* =========================
   STRUCTURE CREATION
========================= */

Structure *create_structure(Node *node, string function_name);

/* =========================
   CHAINED NODE
========================= */

Chained_Node *create_chained_node(Node* node);

/* =========================
   HASH TABLE
========================= */

int hash_index(string key, int taille);
void insert_list(Chained_Node* node, Chained_Node* lst[], int index);
void insert_hash(Node* node, Chained_Node* lst[]);
Chained_Node *lookup_hash(char* key, string function_scope, Chained_Node* lst[]);

/* =========================
   PARAMETERS / FIELDS
========================= */

void field_from_struct(Structure structure);
void parameters_from_function(Function *function);

/* =========================
   DEBUG / PRINT
========================= */

void printFunction(Function *f);
void print_chain(Chained_Node *node);
void print_symbol_tables();

/* =========================
   HELPERS
========================= */

Node *get_function_body(Node *function);

/* =========================
   MAIN SYMBOL TABLE PASS
========================= */

void parse_tree(Node* root);

/* =========================
   PARAMETER DUMP
========================= */

void dump_function_parameter(Chained_Node *node);
void dump_all_parameter();

#endif /* SYMBOL_TABLE_UTILS_H */
