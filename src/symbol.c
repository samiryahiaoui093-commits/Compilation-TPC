
#include "symbol.h"
#include "string.h"


int hash(char* key) {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += key[i];
    }
    return sum % HASH_SIZE;
}

int insert (Node node){
    switch (node.label){
        case NODE_DECLFNCT:
                Variable* elem = (Variable*)malloc(sizeof(Variable));
                Chained_Node* n = (Chained_Node*)malloc(sizeof(Chained_Node));
                elem->definition_line = node.line;
                elem->name = node.identv;


        case NODE_DECLVAR:
        case NODE_DECLVARSTRUCT:
        default:
    }
}

Chained_Node* search(char* key,Chained_Node *table[]) {
    int index = hash(key);
    Chained_Node* current = table[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL; // non trouvé
}
int main(void){

    return 0;
}
