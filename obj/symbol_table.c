#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include "semantic_tests.h"

Chained_Node *global_variable[SYMBOL_TABLE_SIZE]       = {0};
Chained_Node *functions_definitions[SYMBOL_TABLE_SIZE] = {0};
Chained_Node *structure_definitions[SYMBOL_TABLE_SIZE] = {0};

int len_CL(Node *node) {
    int count = 0;
    for (Node *current = node; current; current = current->nextSibling)
        count++;
    return count;
}

void transfer_arguments(Node *list, string arguments_type[], string arguments_name[]) {
    if (!list) return;

    int count = 0;

    for (Node *current = list; current; current = current->nextSibling) {

        switch (current->label) {

            case NODE_TYPE:
                for (Node *c = current->firstChild; c; c = c->nextSibling) {
                    arguments_type[count] = strdup(current->identv);
                    arguments_name[count] = strdup(c->identv);
                    count++;
                }
                break;

            case NODE_STRUCT: {
                Node *struct_ident = current->firstChild;
                if (!struct_ident) break;

                for (Node *c = struct_ident->nextSibling; c; c = c->nextSibling) {
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

int count_structure_field(Node *field_list) {
    int count = 0;

    for (Node *current = field_list; current; current = current->nextSibling) {

        switch (current->label) {

            case NODE_TYPE:
                for (Node *c = current->firstChild; c; c = c->nextSibling)
                    count++;
                break;

            case NODE_STRUCT:
                if (current->firstChild) {
                    for (Node *c = current->firstChild->nextSibling; c; c = c->nextSibling)
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

    for (Node *current = list; current; current = current->nextSibling) {

        switch (current->label) {

            case NODE_TYPE:
                for (Node *c = current->firstChild; c; c = c->nextSibling) {
                    arguments_type[count] = strdup(current->identv);
                    arguments_name[count] = strdup(c->identv);
                    count++;
                }
                break;

            case NODE_STRUCT: {
                Node *struct_name = current->firstChild;
                if (!struct_name) break;

                for (Node *c = struct_name->nextSibling; c; c = c->nextSibling) {
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
int count_arguments(Node *param_list) {
    int count = 0;
    for (Node *p = param_list; p; p = p->nextSibling) {
        if (p->label == NODE_TYPE) {
            for (Node *id = p->firstChild; id; id = id->nextSibling)
                count++;
        } else if (p->label == NODE_STRUCT) {
            Node *struct_name = p->firstChild;
            for (Node *id = struct_name->nextSibling; id; id = id->nextSibling)
                count++;
        }
    }
    return count;
}


Function *create_function(Node *node) {
    if (!node || !node->firstChild) return NULL;

    Node *header    = node->firstChild;
    Node *typeNode  = (header->firstChild && header->firstChild->label == NODE_STRUCT)
                      ? header->firstChild->firstChild
                      : header->firstChild;
    Node *identNode = typeNode ? typeNode->nextSibling : NULL;
    Node *params    = identNode ? identNode->nextSibling : NULL;

    if (!identNode || !identNode->identv) return NULL;

    if (lookup_function(identNode->identv)) {
    fprintf(stderr, "Semantic error: function '%s' already defined\n", identNode->identv);
    error_count++;
}

    if (lookup_hash(identNode->identv, NULL, global_variable)) {
        fprintf(stderr, "Semantic error: '%s' is already declared as a global variable\n", identNode->identv);
        error_count++;
    }

    if (legal_function_name_test(identNode->identv) == TEST_FAILED) {
        fprintf(stderr, "Semantic error: '%s' is a reserved function name\n", identNode->identv);
        error_count++;
    }

    Function *funct = malloc(sizeof(Function));
    if (!funct) return NULL;

    funct->definition_line = header->definition_line;
    funct->name = strdup(identNode->identv);
    funct->type = (typeNode && typeNode->identv)
                  ? strdup(typeNode->identv)
                  : strdup("void");

    int elements = 0;
    if (params && params->firstChild) {
        if (!(params->label == NODE_TYPE && params->identv && strcmp(params->identv, "void") == 0))
            elements = len_CL(params->firstChild);
    }

    funct->argument_count = elements;
    funct->symbol_table   = calloc(SYMBOL_TABLE_SIZE, sizeof(Chained_Node *));

    if (elements > 0) {
       int elements = count_arguments(params->firstChild);
        funct->argument_type  = malloc(sizeof(string) * (elements + 1)); // +1 for sentinel
        funct->argument_names = malloc(sizeof(string) * (elements + 1));
        transfer_arguments(params->firstChild,funct->argument_type,funct->argument_names);
    } else {
        funct->argument_type  = NULL;
        funct->argument_names = NULL;
    }

    return funct;
}

Variable *create_variable(Node *node, string function_name) {
    if (!node || !node->firstChild) return NULL;

    Node *type  = node->firstChild;
    Node *ident = NULL;

    if (type->firstChild)
        ident = type->firstChild;
    else if (type->nextSibling)
        ident = type->nextSibling;

    if (!ident || !ident->identv) return NULL;

    Chained_Node *fn_cn = function_name ? lookup_function(function_name) : NULL;
    Function *fnc = fn_cn ? fn_cn->function : NULL;

    if (lookup_variable(ident->identv, fnc) != NULL) {
        fprintf(stderr,
            "Semantic error: '%s' already declared in scope '%s'\n",
            ident->identv,
            function_name ? function_name : "global");
        error_count++;
    }

    if (!function_name) {
        if (lookup_hash(ident->identv, NULL, global_variable)) {
            fprintf(stderr,
                "Semantic error: global variable '%s' declared more than once\n",
                ident->identv);
            error_count++;
        }
    }
    if (lookup_function(ident->identv)) {
        fprintf(stderr,
            "Semantic error: '%s' is already declared as a function\n",
            ident->identv);
        error_count++;
    }
    if (type->identv && strcmp(type->identv, "void") == 0) {
        fprintf(stderr,
            "Semantic error: variable '%s' cannot have type void\n",
            ident->identv);
        error_count++;
    }

    Variable *variable = malloc(sizeof(Variable));
    if (!variable) return NULL;

    variable->name            = strdup(ident->identv);
    variable->definition_line = type->definition_line;
    variable->type            = type->identv ? strdup(type->identv) : strdup("unknown");
    variable->function_name   = node->function_name ? strdup(node->function_name) : NULL;
    variable->table           = VARIABLE;

    switch (node->kind) {
        case KIND_CHAR: variable->char_value = node->charv; break;
        case KIND_INT:  variable->int_value  = node->intv;  break;
        default: break;
    }

    return variable;
}

Variable *variable_from_parameters(string name, string function_name,int declaration_line, string type) {
    if (!name || !type) return NULL;   
    Variable *variable = malloc(sizeof(Variable));
    if (!variable) return NULL;

    variable->name            = strdup(name);
    variable->definition_line = declaration_line;
    variable->function_name   = function_name ? strdup(function_name) : NULL;
    variable->type            = strdup(type);
    variable->table           = VARIABLE;

    return variable;
}

Structure *create_structure(Node *node, string function_name) {
    if (!node || !node->firstChild) return NULL;

    Node *structure_name = node->firstChild;
    if (!structure_name || !structure_name->nextSibling) return NULL;
    Node *fields = structure_name->nextSibling->firstChild;

    int elements = count_structure_field(fields);

    Structure *structure = malloc(sizeof(Structure));
    if (!structure) return NULL;

    structure->name            = strdup(structure_name->identv);
    structure->definition_line = structure_name->definition_line;
    structure->function_name   = function_name ? strdup(function_name) : NULL;
    structure->table           = STRUCTURE;

    structure->local_fields   = calloc(SYMBOL_TABLE_SIZE, sizeof(Chained_Node *));
    structure->argument_type  = calloc((elements > 0 ? elements + 1 : 2), sizeof(string));
    structure->argument_names = calloc((elements > 0 ? elements + 1 : 2), sizeof(string));

    if (fields)
        transfer_fields(fields, structure->argument_type, structure->argument_names);

    return structure;
}

Chained_Node *create_chained_node(Node *node) {
    if (!node) return NULL;

    Chained_Node *chained_node = malloc(sizeof(Chained_Node));
    if (!chained_node) return NULL;

    chained_node->next         = NULL;
    chained_node->variable     = NULL;
    chained_node->function     = NULL;
    chained_node->structure    = NULL;
    chained_node->symbol_table = NULL;

    switch (node->label) {

        case NODE_DECLFNCT:
        chained_node->tag      = FUNCTION;
        chained_node->function = create_function(node);
        if (!chained_node->function) { free(chained_node); return NULL; }
        chained_node->key          = strdup(chained_node->function->name);
        chained_node->symbol_table = chained_node->function->symbol_table;
        parameters_from_function(chained_node);  // ← dump immédiatement
        break;

        case NODE_DECLSTRUCT:
            chained_node->tag       = STRUCTURE;
            chained_node->structure = create_structure(node, NULL);
            if (!chained_node->structure) { free(chained_node); return NULL; }
            chained_node->key = strdup(chained_node->structure->name);
            break;

        case NODE_DECLVAR:
            chained_node->tag      = VARIABLE;
            chained_node->variable = create_variable(node, node->function_name);
            if (!chained_node->variable) { free(chained_node); return NULL; }
            chained_node->key = strdup(chained_node->variable->name);
            break;

        default:
            free(chained_node);
            return NULL;
    }

    return chained_node;
}

int hash_index(string key, int taille) {
    unsigned int hash = 5381;
    int c;

    if (!key) return 0;

    while ((c = (unsigned char)*key++))
        hash = ((hash << 5) + hash) + c;

    return (int)(hash % (unsigned int)taille);
}

void insert_list(Chained_Node *node, Chained_Node *lst[], int index) {
    if (!node) return;

    if (!lst[index]) {
        lst[index] = node;
        return;
    }

    Chained_Node *c = lst[index];
    while (c->next)
        c = c->next;
    c->next = node;
}

void insert_hash(Node *node, Chained_Node *lst[]) {
    if (!node) return;
    if (node->label == NODE_DECLVAR) {
        for (Node *current = node->firstChild->nextSibling; current; current = current->nextSibling) {
            int idx = hash_index(current->identv, SYMBOL_TABLE_SIZE);
            for (Chained_Node *c = lst[idx]; c; c = c->next) {
                if (strcmp(c->key, current->identv) != 0) continue;
                if (lst == global_variable) {
                    fprintf(stderr, "Semantic error: global variable '%s' already declared\n", current->identv);
                    error_count++;
                } else if (c->variable && c->variable->function_name && node->function_name &&
                           strcmp(c->variable->function_name, node->function_name) == 0) {
                    fprintf(stderr, "Semantic error: '%s' already declared in function '%s'\n", current->identv, node->function_name);
                    error_count++;
                }
            }
            Chained_Node *cn = malloc(sizeof(Chained_Node));
            if (!cn) continue;
            cn->tag          = VARIABLE;
            cn->next         = NULL;
            cn->function     = NULL;
            cn->structure    = NULL;
            cn->symbol_table = NULL;
            cn->variable     = variable_from_parameters(current->identv, node->function_name, node->definition_line, node->firstChild->identv);
            cn->key = strdup(current->identv);
            insert_list(cn, lst, idx);
        }
    } else {
        Chained_Node *elem = create_chained_node(node);
        if (!elem || !elem->key) return;
        int index = hash_index(elem->key, SYMBOL_TABLE_SIZE);
        insert_list(elem, lst, index);
    }
}
Chained_Node *lookup_hash(char *key, string function_scope, Chained_Node *lst[]) {
    if (!key) return NULL;

    int index = hash_index(key, SYMBOL_TABLE_SIZE);

    for (Chained_Node *curr = lst[index]; curr; curr = curr->next) {
        if (!curr->key || strcmp(curr->key, key) != 0) continue;


        switch (curr->tag) {

            case VARIABLE:
                if (!function_scope)
                    return curr;
                if (curr->variable &&
                    curr->variable->function_name &&
                    strcmp(curr->variable->function_name, function_scope) == 0)
                    return curr;
                break;

            case FUNCTION:
            case STRUCTURE:
                return curr;

            default:
                break;
        }
    }

    return NULL;
}


Chained_Node *lookup_struct_field(string field_name, Structure *structure) {
    if (!field_name || !structure || !structure->local_fields)
        return NULL;

    return lookup_hash(field_name, NULL, structure->local_fields);
}

void fields_from_structure(Chained_Node *cns) {
    if (!cns || cns->tag != STRUCTURE || !cns->structure) return;

    Structure *s = cns->structure;

    for (int i = 0; s->argument_names[i] != NULL; i++) {
        Variable *field_var = variable_from_parameters(
            s->argument_names[i],
            NULL,
            s->definition_line,
            s->argument_type[i]);

        Chained_Node *cn = malloc(sizeof(Chained_Node));
        if (!cn) continue;

        cn->tag          = VARIABLE;
        cn->key          = strdup(s->argument_names[i]);
        cn->next         = NULL;
        cn->variable     = field_var;
        cn->function     = NULL;
        cn->structure    = NULL;
        cn->symbol_table = NULL;

        int index = hash_index(s->argument_names[i], SYMBOL_TABLE_SIZE);
        insert_list(cn, s->local_fields, index);
    }
}

void field_from_struct(Structure structure) {
    (void)structure;
}

void parameters_from_function(Chained_Node *cnf) {
    if (!cnf || cnf->tag != FUNCTION || !cnf->function) return;

    Function *f = cnf->function;

    for (int i = 0; i < f->argument_count; i++) {
        Variable *parameter = variable_from_parameters(
            f->argument_names[i],
            f->name,
            f->definition_line,
            f->argument_type[i]);

        Chained_Node *cn = malloc(sizeof(Chained_Node));
        if (!cn) continue;

        cn->tag          = VARIABLE;
        cn->key          = strdup(f->argument_names[i]);
        cn->next         = NULL;
        cn->variable     = parameter;
        cn->function     = NULL;
        cn->structure    = NULL;
        cn->symbol_table = NULL;

        int index = hash_index(f->argument_names[i], SYMBOL_TABLE_SIZE);
        insert_list(cn, cnf->symbol_table, index);
    }
}

void printFunction(Function *f) {
    if (!f) { printf("Function is NULL\n"); return; }

    printf("\n=== Function Analysis ===\n");
    printf("Name: %s\n", f->name);
    printf("Return type: %s\n", f->type);
    printf("Defined at line: %d\n", f->definition_line);
    printf("Arguments (%d):\n", f->argument_count);

    for (int i = 0; i < f->argument_count; i++)
        printf("  - %s %s\n", f->argument_type[i], f->argument_names[i]);

    printf("========================\n");
}

void print_chain(Chained_Node *node, int indent) {
    for (Chained_Node *c = node; c; c = c->next) {

        for (int i = 0; i < indent; i++) putchar(' ');

        switch (c->tag) {

            case VARIABLE:
                if (c->variable) {
                    printf("[VAR] %s : %s (line %d)",
                           c->variable->name,
                           c->variable->type,
                           c->variable->definition_line);
                    if (c->variable->function_name)
                        printf(" function - %s", c->variable->function_name);
                    putchar('\n');
                }
                break;

            case FUNCTION:
                if (c->function) {
                    printf("[FUNC] %s : %s (args=%d, line %d)\n",
                           c->function->name,
                           c->function->type,
                           c->function->argument_count,
                           c->function->definition_line);

                    if (c->symbol_table) {
                        for (int i = 0; i < indent + 4; i++) putchar(' ');
                        printf("Local Symbol Table:\n");

                        for (int i = 0; i < SYMBOL_TABLE_SIZE; i++)
                            if (c->symbol_table[i])
                                print_chain(c->symbol_table[i], indent + 8);
                    }
                }
                break;

            case STRUCTURE:
                if (c->structure)
                    printf("[STRUCT] %s (line %d)\n",
                           c->structure->name,
                           c->structure->definition_line);
                break;

            default:
                break;
        }
    }
}

void print_symbol_tables() {
    printf("\n========== GLOBAL VARIABLES ==========\n");
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++)
        if (global_variable[i])
            print_chain(global_variable[i], 0);

    printf("\n========== FUNCTIONS ==========\n");
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++)
        if (functions_definitions[i])
            print_chain(functions_definitions[i], 0);

    printf("\n========== STRUCTURES ==========\n");
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++)
        if (structure_definitions[i])
            print_chain(structure_definitions[i], 0);

    printf("\n======================================\n");
}

Node *get_function_body(Node *function) {
    if (!function || function->label != NODE_DECLFNCT || !function->firstChild)
        return NULL;
    return function->firstChild->nextSibling;
}
void parse_tree(Node *root) {
    if (!root || root->label != NODE_PROGRAM) return;
    Node *global_decls = root->firstChild;
    if (global_decls && global_decls->label == NODE_VARS && global_decls->firstChild)
        for (Node *curr = global_decls->firstChild; curr; curr = curr->nextSibling)
            if (curr) {
                if (curr->label == NODE_DECLVAR) insert_hash(curr, global_variable);
                else if (curr->label == NODE_DECLSTRUCT) insert_hash(curr, structure_definitions);
            }
    Node *fn_section = global_decls ? global_decls->nextSibling : root->firstChild;
    while (fn_section) {
        if (fn_section->label == NODE_FNCS || fn_section->label == NODE_DECLFNCT) {
            Node *fn_to_parse = (fn_section->label == NODE_FNCS) ? fn_section->firstChild : fn_section;
            if (!fn_to_parse) goto next_section;
            for (Node *curr_fn = fn_to_parse; curr_fn; curr_fn = curr_fn->nextSibling) {
                if (!curr_fn || curr_fn->label != NODE_DECLFNCT) continue;
                Node *header = curr_fn->firstChild;
                if (!header) continue;
                Node *typeNode = (header->firstChild && header->firstChild->label == NODE_STRUCT) ? header->firstChild->firstChild : header->firstChild;
                if (!typeNode) continue;
                Node *identNode = typeNode->nextSibling;
                if (!identNode || !identNode->identv) continue;
                insert_hash(curr_fn, functions_definitions);
                Node *body = get_function_body(curr_fn);
                if (!body || !body->firstChild) continue;
                Chained_Node *cn_func = lookup_function(identNode->identv);
                if (!cn_func || !cn_func->symbol_table) continue;
                if (body->firstChild->label == NODE_VARS && body->firstChild->firstChild)
                    for (Node *decl_var = body->firstChild->firstChild; decl_var; decl_var = decl_var->nextSibling)
                        if (decl_var && decl_var->label == NODE_DECLVAR) insert_hash(decl_var, cn_func->symbol_table);
            }
        }
        next_section:
        fn_section = fn_section->nextSibling;
    }
}

void dump_function_parameter(Chained_Node *node) {
    for (Chained_Node *c = node; c; c = c->next)
        parameters_from_function(c);
}

void dump_structure_fields(Chained_Node *node) {
    for (Chained_Node *c = node; c; c = c->next) {
        if (c->tag == STRUCTURE) {
            fields_from_structure(c);
        }
    }
}

void dump_all_parameter() {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        printf("%d", i);
        dump_function_parameter(functions_definitions[i]);
        dump_structure_fields(structure_definitions[i]);
    }
}

void function_from_parameters(string name, string type, int definition_line, string argument_names[], string arguments_type[], int argument_count) {
    Function *func = malloc(sizeof(Function));
    if (!func) return ;
    func->name = strdup(name);
    func->type = strdup(type);
    func->definition_line = definition_line;
    func->argument_count = argument_count;
    func->symbol_table = calloc(SYMBOL_TABLE_SIZE, sizeof(Chained_Node *));
    if (argument_count > 0) {
        func->argument_type  = malloc(sizeof(string) * argument_count);
        func->argument_names = malloc(sizeof(string) * argument_count);
        for (int i = 0; i < argument_count; i++) {
            func->argument_type[i]  = strdup(arguments_type[i]);
            func->argument_names[i] = strdup(argument_names[i]);
        }
    } else {
        func->argument_type  = NULL;
        func->argument_names = NULL;
    }
    Chained_Node * cn = (Chained_Node*)malloc(sizeof(Chained_Node));
    if (!cn) {
        return;
    }
    cn->function     = func;
    cn->key          = strdup(name);
    cn->tag          = FUNCTION;
    cn->next         = NULL;
    cn->variable     = NULL;
    cn->structure    = NULL;
    cn->symbol_table = func->symbol_table;
    int index = hash_index(name, SYMBOL_TABLE_SIZE);
    insert_list(cn, functions_definitions, index);
}

void dump_essentials() {
    string putchar_arg[]  = {"c"};
    string putchar_type[] = {"char"};
    string putint_arg[]   = {"n"};
    string putint_type[]  = {"int"};
    function_from_parameters("putchar", "void", 0, putchar_arg, putchar_type, 1);
    function_from_parameters("putint",  "void", 0, putint_arg,  putint_type,  1);
    function_from_parameters("getint",  "int",  0, NULL, NULL, 0);
    function_from_parameters("getchar", "int",  0, NULL, NULL, 0);
}