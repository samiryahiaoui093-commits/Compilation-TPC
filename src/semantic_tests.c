#include "ast.h"
#include "symbol_table.h"
#include <stdio.h>
#include "semantic_tests.h"    
#include <string.h>

int error_count = 0;


/*
Test a faire -


-appel recursif autorisé

-appel de variable du bon scope.
- a ssignation du bon type
-opération avec les bon type
- Dump dans la table des symbole les parametre
Comme en C, une variable globale et une fonction ne peuvent pas avoir le même nom, et deux fonctions non
plus.
Comme en C, une variable locale et un paramètre d’une même fonction ne peuvent pas avoir le même nom.
Une variable globale ne peut être déclarée qu’une fois (2)

-Fonction qui vont etre utile
-lookup(name,function_scope) pour vérifié que la variable est bien défini dans le bon scope
- check_function call(Node* function_call_node,table function) pour vérifier l'ordre
-la récusivité est normalement ok avec ma table
- check_struct_field() pour vérifier qu'un champ de structure est bien fait
-
- create_variable (name,type);

presence du main bien initialisé
pas pouvoir utiliser une fonction comme une variable  et une variable comme une fonction
vérifier qu'un appel de fonction peut bien etre utiliser dans une expression
-- La table des symbole -
dump les champs des structure dans les fields
dump les parametre de fonction dans les variable locale


-regarder si je pourrais pas simplifier l'implémentation
-pourrait faire des variable global pour les erreur

*/

static Node* seen[100000];
static int seen_count = 0;

int already_seen(Node* n) {
    for (int i = 0; i < seen_count; i++)
        if (seen[i] == n) return 1;

    seen[seen_count++] = n;
    return 0;
}

Chained_Node *lookup_function(string function_name) {
    if (!function_name)
        return NULL;

    return lookup_hash(function_name, NULL,functions_definitions);
}

Chained_Node *lookup_variable(string variable_name, Function *function) {
    if (!variable_name)
        return NULL;
    Chained_Node *result = NULL;
    if (function && function->symbol_table) {
        result = lookup_hash(variable_name, function->name, function->symbol_table);

        if (result)
            return result;
    }

    result = lookup_hash(variable_name,NULL,global_variable);

    return result;
}



Chained_Node* lookup(string variable_name, string function_scope, Chained_Node* symbol_table[]) {
    (void)symbol_table; // unused, kept for compatibility

    if (!variable_name)
        return NULL;

    // 1) Local scope (function parameters and local variables)
    if (function_scope) {
        Chained_Node *func_cn = lookup_function(function_scope);
        if (func_cn && func_cn->function && func_cn->function->symbol_table) {
            Chained_Node *local = lookup_hash(variable_name, function_scope, func_cn->function->symbol_table);
            if (local && local->tag == VARIABLE)
                return local;
        }
    }

    // 2) Global variables
    Chained_Node *global = lookup_hash(variable_name, NULL, global_variable);
    if (global && global->tag == VARIABLE)
        return global;

    return NULL;
}
int assign_field_test(){
    return 0;
}
// Fonction pour tester si les variable utilisé dans la L-value
int assign_test(Node *assign_node, string function_name, Chained_Node *symbol_table[]) {
    Chained_Node *fn_cn = lookup_function(function_name);
    Function *fnc = fn_cn ? fn_cn->function : NULL;
    Chained_Node *vr = lookup_variable(assign_node->firstChild->identv, fnc);
    if (vr && vr->variable && vr->variable->definition_line <= assign_node->line)
        return TEST_PASSED;
    return TEST_FAILED;
}
string get_type(string variable_name, string function_name, Chained_Node * symbol_table[]){
    Chained_Node *var = lookup(variable_name, function_name, symbol_table);
    if(var && var->tag == VARIABLE && var->variable) {
        return var->variable->type;
    }
    return NULL;
}
int function_call_test(Node * function_call,string function_name,Chained_Node * symbol_table[]){
    Node * function_ident = function_call->firstChild;
    Node * function_arguments = function_ident->nextSibling;
    int argument_number = len_CL(function_arguments);
    Chained_Node * cn =lookup(function_call->firstChild->identv,function_name,symbol_table);
    if(cn == NULL || cn->function == NULL) return TEST_FAILED;
    if (cn->function->argument_count != argument_number){
        return TEST_FAILED;
    } else {
        int index = 0;
        for(Node *current = function_arguments;current;current = current->nextSibling){
            if (strcmp(get_type(current->identv,function_name,symbol_table),cn->function->argument_type[index]) != 0){
                return TEST_FAILED;
            }
        }

    }
    if( cn && cn->function->definition_line <= function_call->line){
        return TEST_PASSED;
    }
    return TEST_FAILED;
}


int return_value_test(Node* return_node, string function_name) {
    Chained_Node *cn = lookup_function(function_name);
    if (!cn || !cn->function) return TEST_FAILED;
    string fn_type = cn->function->type;

    if (!return_node->firstChild) {
        return (strcmp(fn_type, "void") == 0) ? TEST_PASSED : TEST_FAILED;
    }

    switch (return_node->firstChild->label) {
        case NODE_NUM:
            return (strcmp(fn_type, "int") == 0) ? TEST_PASSED : TEST_FAILED;

        case NODE_CHAR:
            return (strcmp(fn_type, "char") == 0) ? TEST_PASSED : TEST_FAILED;

        case NODE_IDENT: {
            Chained_Node *var = lookup_variable(return_node->firstChild->identv, cn->function);
            if (!var || !var->variable) return TEST_FAILED;
            return (strcmp(fn_type, var->variable->type) == 0) ? TEST_PASSED : TEST_FAILED;
        }
        case NODE_FUNCTION_CALL: {
            Chained_Node *called = lookup_function(return_node->firstChild->firstChild->identv);
            if (!called || !called->function) return TEST_PASSED;
                return (strcmp(fn_type, called->function->type) == 0) ? TEST_PASSED : TEST_FAILED;
        }
;
        default:
            return TEST_PASSED;
    }
}

int expression_evaluation_test(Node * expression){
    return 0;
}

void parse_instr(Node* instr,int *error_count,string function_name){

    for(Node *current = instr; current ; current = current->nextSibling){
        printf("NODE %p label=%d next=%p child=%p\n",
        current,
        current->label,
        current->nextSibling,
        current->firstChild);
        if (already_seen(current)) {
    printf("🚨 CYCLE DETECTED: %p\n", current);
    return;
}
        switch (current->label) {
            case NODE_ASSIGN:
                (*error_count)+= assign_test(current,function_name,functions_definitions);
                break;

            case NODE_FUNCTION_CALL:
                (*error_count)+= function_call_test(current,function_name, functions_definitions);
                break;
           case NODE_IF:
                if (current->firstChild && current->firstChild->nextSibling) {
                    parse_instr(current->firstChild->nextSibling, error_count, function_name);
                }
                if (current->nextSibling && current->nextSibling->label == NODE_ELSE &&
                    current->nextSibling->firstChild) {
                    parse_instr(current->nextSibling->firstChild, error_count, function_name);
                }
                break;
            case NODE_RETURN:
                (*error_count)+=return_value_test(current,function_name);
                break;

            default:
                    break;
        }
    }
}

int global_name_duplicate_test(string name){
    if(lookup(name,NULL,functions_definitions) || lookup(name,NULL,global_variable)  ) {
        return TEST_FAILED;
    }
    return TEST_PASSED;
}
Node * get_instr(Node * node);
int legal_local_variable_name_test(string name,string function_name,Chained_Node *symbol_table[]){
    Chained_Node* cn = lookup_hash(name, function_name,symbol_table);
    if (cn){
        return TEST_FAILED;
    }
    return TEST_PASSED;
}

// getint getchar putchar put int
int legal_function_name_test(string name){
    string illegal_name[] = {"getint","getchar","putchar","putint"};
    for(int i = 0; i < 4 ; i ++){
        if(strcmp(name,illegal_name[i]) == 0){
            return TEST_FAILED;
        }}
    return TEST_PASSED;
}
int main_test(){
    Chained_Node *cn = lookup_function("main");
    if(cn && cn->function){
        return ((lookup_function("main") && strcmp(lookup_function("main") ->function->type,"int") == 0 )? TEST_PASSED : TEST_FAILED );
    }
    return TEST_FAILED;
}

void test_tree(Node* root, int *error_count) {
    if (!root || root->label != NODE_PROGRAM) return;
    *error_count += main_test();
    Node *global_decls = root->firstChild;
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
                Node *body = get_function_body(curr_fn);
                if (!body || !body->firstChild) continue;
                Node *instr_start = body->firstChild;
                if (instr_start->label == NODE_VARS) instr_start = instr_start->nextSibling;
                if (!instr_start || !instr_start->firstChild) continue;
                for (Node *current_instruction = instr_start->firstChild; current_instruction; current_instruction = current_instruction->nextSibling)
                    parse_instr(current_instruction, error_count, identNode->identv);
            }
        }
        next_section:
        fn_section = fn_section->nextSibling;
    }
}