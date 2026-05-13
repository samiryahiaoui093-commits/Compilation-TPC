#include <bits/posix2_lim.h>
#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"
#include <string.h>
#include <strings.h>
#include "semantic_tests.h"

/* =========================
   GLOBAL TABLES
========================= */

Chained_Node *global_variable[SYMBOL_TABLE_SIZE] = {0};
Chained_Node *functions_definitions[SYMBOL_TABLE_SIZE] = {0};
Chained_Node *structure_definitions[SYMBOL_TABLE_SIZE] = {0};

/* =========================
   UTILITIES
========================= */

int len_CL(Node* node) {
    int count = 0;
    for (Node* current = node; current; current = current->nextSibling)
        count++;
    return count;
}

int argument_count(Node* node) {
    int count = 0;
    for (Node* current = node; current; current = current->nextSibling){
        for (Node* c = current; c; c = c->nextSibling){
            count++;
        }
    }
    return count;
}
/* =========================
   ARGUMENTS
========================= */


void transfer_arguments(Node *list, string arguments_type[], string arguments_name[]) {
    if (!list) return;

    int count = 0;

    for (Node* current = list; current; current = current->nextSibling) {
        if (!current) continue;

        switch(current->label) {

            case NODE_TYPE:
                for (Node* c = current->firstChild; c; c = c->nextSibling) {
                    if (!c) continue;
                    arguments_type[count] = strdup(current->identv);
                    arguments_name[count] = strdup(c->identv);
                    count++;
                }
                break;

            case NODE_STRUCT: {
                Node *struct_ident = current->firstChild;
                if (!struct_ident) break;

                for (Node* c = struct_ident->nextSibling; c; c = c->nextSibling) {
                    if (!c) continue;
                    arguments_type[count] = strdup(struct_ident->identv);
                    arguments_name[count] = strdup(c->identv);
                    count++;
                }
                break;
            }

            default:
                break;
        }
    }
}

/* =========================
   STRUCT FIELDS
========================= */

int count_structure_field(Node *field_list) {
    int count = 0;

    for (Node* current = field_list; current; current = current->nextSibling) {
        if (!current) continue;

        switch(current->label) {

            case NODE_TYPE:
                for (Node* c = current->firstChild; c; c = c->nextSibling)
                    count++;
                break;

            case NODE_STRUCT:
                if (current->firstChild) {
                    for (Node* c = current->firstChild->nextSibling; c; c = c->nextSibling)
                        count++;
                }
                break;

            default:
                break;
        }
    }

    return count;
}

void transfer_fields(Node *list, string arguments_type[], string arguments_name[]) {
    if (!list) return;

    if (list->firstChild &&
        list->firstChild->identv &&
        strcmp(list->firstChild->identv, "void") == 0)
        return;

    int count = 0;

    for (Node* current = list; current; current = current->nextSibling) {
        if (!current) continue;

        switch(current->label) {

            case NODE_TYPE:
                for (Node *c = current->firstChild; c; c = c->nextSibling) {
                    if (!c) continue;
                    arguments_type[count] = strdup(current->identv);
                    arguments_name[count] = strdup(c->identv);
                    count++;
                }
                break;

            case NODE_STRUCT: {
                Node *struct_name = current->firstChild;
                if (!struct_name) break;

                for (Node *c = struct_name->nextSibling; c; c = c->nextSibling) {
                    if (!c) continue;
                    arguments_type[count] = strdup(struct_name->identv);
                    arguments_name[count] = strdup(c->identv);
                    count++;
                }
                break;
            }

            default:
                break;
        }
    }
}

/* =========================
   FUNCTION CREATION
========================= */

Function *create_function(Node* node) {
    if (!node || !node->firstChild) return NULL;

    Node *header = node->firstChild;

    Node *typeNode =
        (header->firstChild && header->firstChild->label == NODE_STRUCT)
        ? header->firstChild->firstChild
        : header->firstChild;

    Node *identNode = (typeNode) ? typeNode->nextSibling : NULL;
    Node *params = (identNode) ? identNode->nextSibling : NULL;

    Function* funct = malloc(sizeof(Function));
    if (!funct) return NULL;

    funct->definition_line = node->firstChild->definition_line;

    funct->name = (identNode && identNode->identv)
        ? strdup(identNode->identv)
        : strdup("error");

    funct->type = (typeNode && typeNode->identv)
        ? strdup(typeNode->identv)
        : strdup("void");

    int elements = 0;
    if (params && params->firstChild)
        elements = len_CL(params->firstChild);

    funct->argument_count = (elements > 0)? elements : elements ;

    if (elements > 0) {
        funct->argument_type = malloc(sizeof(string) * elements);
        funct->argument_names = malloc(sizeof(string) * elements);

        if (params && params->firstChild)
            transfer_arguments(params->firstChild,
                                funct->argument_type,
                                funct->argument_names);
    } else {
        funct->argument_type = NULL;
        funct->argument_names = NULL;
    }

    return funct;
}

/* =========================
   VARIABLE CREATION
========================= */

Variable *create_variable(Node* node, string function_name) {
    if (!node || !node->firstChild) return NULL;

    Node *type = node->firstChild;
    Node *ident = NULL;

    if (type->firstChild)
        ident = type->firstChild;
    else if (type->nextSibling)
        ident = type->nextSibling;

    if (!ident || !ident->identv) return NULL;

    Variable *variable = malloc(sizeof(Variable));
    if (!variable) return NULL;

    variable->name = ident->identv;
    variable->definition_line = type->definition_line;
    variable->type = strdup(type->identv);
    variable->function_name = node->function_name ? strdup(node->function_name) : NULL;
    switch(node->kind) {
        case KIND_CHAR:
            variable->char_value = node->charv;
            break;
        case KIND_INT:
            variable->int_value = node->intv;
            break;
        default:
            break;
    }

    return variable;
}
Variable * variable_from_parameters(string name,string function_name, int declaration_line,string type){
    Variable *variable = malloc(sizeof(Variable));
    if (!variable) return NULL;
    variable->name = strdup(name);
    variable->definition_line= declaration_line;
    variable->function_name = strdup(function_name);
    variable->type = strdup(type);
    variable->table = VARIABLE;
    return variable;
}

/* =========================
   STRUCTURE CREATION
========================= */

Structure *create_structure(Node *node, string function_name) {
    if (!node || !node->firstChild) return NULL;

    Node *structure_name = node->firstChild;

    Node *fields = NULL;
    if (structure_name->nextSibling)
        fields = structure_name->nextSibling->firstChild;

    int elements = count_structure_field(fields);

    Structure* structure = malloc(sizeof(Structure));
    if (!structure) return NULL;

    structure->name = structure_name->identv;
    structure->definition_line = structure_name->definition_line;

    structure->argument_type = malloc(sizeof(string) * elements);
    structure->argument_names = malloc(sizeof(string) * elements);

    structure->function_name = function_name;

    if (fields)
        transfer_fields(fields,
                        structure->argument_type,
                        structure->argument_names);

    return structure;
}

/* =========================
   CHAINED NODE
========================= */

Chained_Node *create_chained_node(Node* node) {
    if (!node) return NULL;

    Chained_Node* chained_node = malloc(sizeof(Chained_Node));
    if (!chained_node) return NULL;

    switch(node->label) {

        case NODE_DECLFNCT:
            chained_node->tag = FUNCTION;
            chained_node->function = create_function(node);
            if (!chained_node->function) return NULL;
            chained_node->key = strdup(chained_node->function->name);
            chained_node->symbol_table = calloc(SYMBOL_TABLE_SIZE, sizeof(Chained_Node*));
            break;

        case NODE_DECLSTRUCT:
            chained_node->tag = STRUCTURE;
            chained_node->structure = create_structure(node, NULL);
            if (!chained_node->structure) return NULL;
            chained_node->key = strdup(chained_node->structure->name);
            break;

        case NODE_DECLVAR:
                chained_node->tag = VARIABLE;
                chained_node->variable = create_variable(node,node->function_name);   // plus de NULL
                if (!chained_node->variable) return NULL;
                chained_node->key = strdup(chained_node->variable->name);
                break;

        default:
            free(chained_node);
            return NULL;
    }

    chained_node->next = NULL;
    return chained_node;
}

/* =========================
   HASH TABLE
========================= */

int hash_index(string key, int taille) {
    unsigned int hash = 5381;
    int c;

    if (!key) return 0;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;

    return hash % taille;
}

void insert_list(Chained_Node* node, Chained_Node* lst[], int index) {
    if (!node) return;

    if (lst[index] == NULL) {
        lst[index] = node;
    } else {
        for (Chained_Node *c = lst[index]; c; c = c->next) {
            if (c->next == NULL) {
                c->next = node;
                return;
            }
        }
    }
}

void insert_hash(Node* node, Chained_Node* lst[]) {
    if (!node) return;
    Chained_Node *elem;
    if(node->label == NODE_DECLVAR){
        for(Node* current = node->firstChild->nextSibling;current;current = current->nextSibling){

            Chained_Node * cn = (Chained_Node*)malloc(sizeof(Chained_Node));
            int index = hash_index(current->identv, SYMBOL_TABLE_SIZE);
            cn->variable = variable_from_parameters(current->identv,node->function_name,node->definition_line,node->firstChild->identv);
            cn->key = strdup(current->identv);
            cn->tag = VARIABLE;
            cn->next = NULL;
            insert_list(cn, lst, index);


        }

    } else {
    elem = create_chained_node(node);
    if (!elem || !elem->key) return;
    int index = hash_index(elem->key, SYMBOL_TABLE_SIZE);
    insert_list(elem, lst, index);
    }

}

Chained_Node *lookup_hash(char* key,
                          string function_scope,
                          Chained_Node* lst[]) {

    if (!key)
        return NULL;

    int index = hash_index(key, SYMBOL_TABLE_SIZE);

    Chained_Node* curr = lst[index];

    while (curr) {

        if (strcmp(curr->key, key) == 0) {
            if (curr->tag == VARIABLE) {
                if (!function_scope)
                    return curr;            /* VARIABLES */
                if (curr->variable && curr->variable->function_name && strcmp(curr->variable->function_name,function_scope) == 0) {
                    return curr;}
            }
            else if (curr->tag == FUNCTION) {
                return curr;
            }
            else if (curr->tag == STRUCTURE) {
                return curr;
            }}
        curr = curr->next;
    }
    return NULL;
}
void field_from_struct(Structure structure){

}
void parameters_from_function(Chained_Node *cnf){
    Variable * parameter;
    Chained_Node * cn;
    int index_hash;
    for(int i = 0; i <= cnf->function->argument_count -1;i++){
        parameter = variable_from_parameters(cnf->function->argument_names[i],cnf->function->name,cnf->function->definition_line,cnf->function->argument_type[i]);
        cn = malloc(sizeof(Chained_Node));
        cn->key = strdup(cnf->function->argument_names[i]);;
        cn->next = NULL;
        cn->tag = VARIABLE;
        cn->variable = parameter;
        index_hash = hash_index(cnf->function->argument_names[i], SYMBOL_TABLE_SIZE);
        insert_list(cn,cnf->symbol_table,index_hash);
    }
}
/* =========================
   FUNCTION DEBUG
========================= */

void printFunction(Function *f) {
    if (!f) {
        printf("Function is NULL\n");
        return;
    }

    printf("\n=== Function Analysis ===\n");
    printf("Name: %s\n", f->name);
    printf("Return type: %s\n", f->type);
    printf("Defined at line: %d\n", f->definition_line);

    printf("Arguments (%d):\n", f->argument_count);
    for (int i = 0; i < f->argument_count; i++) {
        printf("  - %s %s\n", f->argument_type[i], f->argument_names[i]);
    }
    printf("========================\n");
}

/* =========================
   HELPERS
========================= */

Node *get_function_body(Node *function) {
    return (function && function->label == NODE_DECLFNCT)
        ? function->firstChild->nextSibling
        : NULL;
}



void parse_tree(Node* root) {
    if (!root || root->label != NODE_PROGRAM) return;

    Node *global_decls = root->firstChild;

    if (global_decls && global_decls->label == NODE_VARS) {
        for (Node *curr = global_decls->firstChild; curr; curr = curr->nextSibling) {
            if (curr->label == NODE_DECLVAR)
                insert_hash(curr, global_variable);
            else if (curr->label == NODE_DECLSTRUCT)
                insert_hash(curr, structure_definitions);
        }
    }

    Node *fn_section = (global_decls) ? global_decls->nextSibling : root->firstChild;

    while (fn_section) {
        if (fn_section->label == NODE_FNCS || fn_section->label == NODE_DECLFNCT) {

            Node *fn_to_parse =
                (fn_section->label == NODE_FNCS)
                ? fn_section->firstChild
                : fn_section;
            Node *header = fn_to_parse->firstChild;

            

            for (Node *curr_fn = fn_to_parse; curr_fn; curr_fn = curr_fn->nextSibling) {

    if (curr_fn->label != NODE_DECLFNCT)
        continue;

    Node *header = curr_fn->firstChild;

    Node *typeNode =
        (header->firstChild &&
         header->firstChild->label == NODE_STRUCT)
        ? header->firstChild->firstChild
        : header->firstChild;

    Node *identNode =
        (typeNode) ? typeNode->nextSibling : NULL;

    insert_hash(curr_fn, functions_definitions);

    Node *body = get_function_body(curr_fn);

    if (body &&
        body->firstChild &&
        body->firstChild->label == NODE_VARS) {

        for (Node *l_var = body->firstChild->firstChild;
             l_var;
             l_var = l_var->nextSibling) {

            Chained_Node *cn =
                lookup_hash(
                    identNode->identv,
                    identNode->identv,
                    functions_definitions
                );

            if (cn) {
                insert_hash(l_var, cn->symbol_table);
            }
        }
    }
}
        }

        fn_section = fn_section->nextSibling;
    }
}

/*
for (Node *curr_fn = fn_to_parse; curr_fn; curr_fn = curr_fn->nextSibling) {

    if (curr_fn->label != NODE_DECLFNCT)
        continue;

    Node *header = curr_fn->firstChild;

    Node *typeNode =
        (header->firstChild &&
         header->firstChild->label == NODE_STRUCT)
        ? header->firstChild->firstChild
        : header->firstChild;

    Node *identNode =
        (typeNode) ? typeNode->nextSibling : NULL;

    insert_hash(curr_fn, functions_definitions);

    Node *body = get_function_body(curr_fn);

    if (body &&
        body->firstChild &&
        body->firstChild->label == NODE_VARS) {

        for (Node *l_var = body->firstChild->firstChild;
             l_var;
             l_var = l_var->nextSibling) {

            Chained_Node *cn =
                lookup_hash(
                    identNode->identv,
                    identNode->identv,
                    functions_definitions
                );

            if (cn) {
                insert_hash(l_var, cn->symbol_table);
            }
        }
    }
}
*/

void dump_function_parameter(Chained_Node * node){
    if(node){
        for (Chained_Node *c = node; c; c = c->next) {
            if (c)
            parameters_from_function(c);

            }

    }


}
void dump_all_parameter(){
    for(int i = 0; i < SYMBOL_TABLE_SIZE ; i++){
        printf("%d",i);
        dump_function_parameter(functions_definitions[i]);
    }
}
void print_chain(Chained_Node *node, int indent) {
    for (Chained_Node *c = node; c; c = c->next) {

        for (int i = 0; i < indent; i++)
            printf(" ");

        switch (c->tag) {

            case VARIABLE:
                if (c->variable) {
                    printf("[VAR] %s : %s (line %d)",
                        c->variable->name,
                        c->variable->type,
                        c->variable->definition_line);

                    if (c->variable->function_name)
                        printf(" function - %s", c->variable->function_name);

                    printf("\n");
                }
                break;

            case FUNCTION:
                if (c->function) {

                    printf("[FUNC] %s : %s (args=%d, line %d)\n",
                        c->function->name,
                        c->function->type,
                        c->function->argument_count,
                        c->function->definition_line);

                    /* afficher la table locale de la fonction */
                    if (c->symbol_table) {

                        for (int i = 0; i < indent + 4; i++)
                            printf(" ");

                        printf("Local Symbol Table:\n");

                        for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
                            if (c->symbol_table[i]) {
                                print_chain(c->symbol_table[i], indent + 8);
                            }
                        }
                    }
                }
                break;

            case STRUCTURE:
                if (c->structure) {
                    printf("[STRUCT] %s (line %d)\n",
                        c->structure->name,
                        c->structure->definition_line);
                }
                break;

            default:
                break;
        }
    }
}


/* =========================
   PRINT FULL TABLE
========================= */

void print_symbol_tables() {

    printf("\n========== GLOBAL VARIABLES ==========\n");
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        if (global_variable[i])
            print_chain(global_variable[i], 0);
    }

    printf("\n========== FUNCTIONS ==========\n");
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        if (functions_definitions[i])
            print_chain(functions_definitions[i], 0);
    }

    printf("\n========== STRUCTURES ==========\n");
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        if (structure_definitions[i])
            print_chain(structure_definitions[i], 0);
    }

    printf("\n======================================\n");
}
