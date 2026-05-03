#include "ast.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include "symbol_table.c"

/*
Test a faire -

-assignasion de variable bien défini
-appel correcte des fonction  avec le bon nombre d'argument et les bon type
-appel recursif autorisé
-appel de fonction défini
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




-- La table des symbole -
dump les champs des structure dans les fields
dump les parametre de fonction dans les variable locale


-regarder si je pourrais pas simplifier l'implémentation
-pourrait faire des variable global pour les erreur

*/



int lookup(string variable_name,string function_scope,Chained_Node* symbol_table[]){
    Chained_Node *searched = lookup_hash(variable_name,function_scope,symbol_table);
    if (searched) {
        return 1;
    } else {
        return 0;
    }
}

int main(void){


    return 0;
}