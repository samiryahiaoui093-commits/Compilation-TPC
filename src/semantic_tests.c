#include "ast.h"
#include "symbol_table.h"

#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_FAILED 1
#define TEST_PASSED 0
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



Chained_Node* lookup(string variable_name,string function_scope,Chained_Node* symbol_table[]){
    Chained_Node *searched = lookup_hash(variable_name,function_scope,symbol_table);
    if (searched) {
        return searched;
    } else {
        return NULL;
    }
}
int assign_field_test(){

}
// Fonction pour tester si les variable utilisé dans la L-value
int assign_test(Node * assign_node,string function_name,Chained_Node * symbol_table[]){
    Chained_Node * vr = lookup(assign_node->firstChild->identv,function_name,symbol_table);
    if(vr && vr->variable->definition_line < assign_node->line){
        return TEST_PASSED;
    }
    return TEST_FAILED;
}

string get_type(string variable_name, string function_name,Chained_Node * symbol_table[]){
    Chained_Node *var= lookup(variable_name,function_name,symbol_table);
    if(var){
            return var->variable->type;
        }
    return NULL;
}
int function_call_test(Node * function_call,string function_name,Chained_Node * symbol_table[]){
    Node * function_ident = function_call->firstChild;
    Node * function_arguments = function_ident->nextSibling;
    int argument_number = len_CL(function_arguments);
    Chained_Node * cn =lookup(function_call->firstChild->identv,function_name,symbol_table);
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
    if( cn && cn->function->definition_line < function_call->line){
        return TEST_PASSED;
    }
    return TEST_FAILED;
}


int return_value_test(Node* return_node,string function_name){
    Chained_Node * cn ;
    if( return_node->firstChild){
        switch(return_node->firstChild->label){
            case NODE_NUM:
              cn =lookup(function_name, function_name, functions_definitions);
             if( strcmp(cn->variable->type,"int") == 0){
                 return TEST_PASSED;
             } else {
                 return TEST_FAILED;
             }
            case NODE_CHAR:
                 cn =lookup(function_name, function_name, functions_definitions);
                if( strcmp(cn->variable->type,"char") == 0){
                    return TEST_PASSED;
                } else {
                    return TEST_FAILED;
                }
                break;
            case NODE_IDENT:
                    cn =lookup(return_node->firstChild->identv, function_name, local_variable);
                    if( strcmp(cn->variable->type,get_type(return_node->firstChild->identv, function_name, local_variable)) == 0){
                        return TEST_PASSED;
                    } else {
                        return TEST_FAILED;
                    }
                    break;

            default:
        }
    } else {
        cn =lookup(function_name, function_name, functions_definitions);
        if( strcmp(cn->function->type,"void") == 0){
            return TEST_PASSED;
        } else {
            return TEST_FAILED;
        }
    }
}

int expression_evaluation_test(Node * expression){
    return 0;
}

void parse_instr(Node* instr,int *error_count,string function_name){

    for(Node *current = instr; current ; current = current->nextSibling){
        switch (current->label) {
            case NODE_ASSIGN:
                (*error_count)+= assign_test(current,function_name,local_variable);
                break;

            case NODE_FUNCTION_CALL:
                (*error_count)+= function_call_test(current,function_name, functions_definitions);
                break;
            case NODE_IF:
                // potentiel recursivité pour les if
                parse_instr(current->firstChild->nextSibling,error_count,function_name);
                if (current->nextSibling && current->nextSibling->label == NODE_ELSE){
                    parse_instr(current->nextSibling->firstChild,error_count,function_name);
                }
                break;
            case NODE_RETURN:
                (*error_count)+=return_value_test(current,function_name);
                break;

            default:

        }
    }
}

int global_name_duplicate_test(string name){
    if(lookup(name,NULL,functions_definitions) || lookup(name,NULL,global_variable)  ) {
        return TEST_FAILED;
    }
    return TEST_PASSED;
}

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
