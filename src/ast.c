#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addSibling(Node *node, Node *nextSibling) {
    if (!node) return;
    while(node->nextSibling) {
        node = node->nextSibling;
    }
    node->nextSibling = nextSibling;
}

void addChild(Node *node, Node *nextChild) {
    if (!node || !nextChild) return;
    if(!node->firstChild) {
        node->firstChild = nextChild;
    } else {
        addSibling(node->firstChild, nextChild);
    }
}

Node* newNode(label_t label) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (node == NULL) exit(1);

    node->label = label;
    node->kind = KIND_NONE;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    node->identv = NULL;
    node->function_name = NULL;
    node->intv = 0;
    node->charv = 0;
    node->definition_line = 0;


    return node;
}
const char* labelToString(label_t label) {
    switch(label) {
        case NODE_PROGRAM: return "PROGRAM";
        case NODE_NUM: return "NUM";
        case NODE_IDENT: return "IDENT";
        case NODE_ADDSUB: return "ADDSUB";
        case NODE_DIVSTAR: return "DIVSTAR";
        case NODE_VOID: return "VOID";
        case NODE_IF: return "IF";
        case NODE_ELSE: return "ELSE";
        case NODE_RETURN: return "RETURN";
        case NODE_AND: return "AND";
        case NODE_OR: return "OR";
        case NODE_EQ: return "EQ";
        case NODE_ORDER: return "ORDER";
        case NODE_TYPE: return "TYPE";
        case NODE_DECLVAR: return "DECLVAR";
        case NODE_DECLVARSTRUCT: return "DECLVARSTRUCT";
        case NODE_ENTETE: return "ENTETE";
        case NODE_PARAMETRE: return "PARAMETRE";
        case NODE_CORPS: return "CORPS";
        case NODE_INSTR: return "INSTR";
        case NODE_DECLFNCT: return "DECLFNCT";
        case NODE_STRUCT: return "STRUCT";
        case NODE_CHAR: return "CHAR";
        case NODE_WHILE: return "WHILE";
        case NODE_FUNCTION: return "FUNCTION";
        case NODE_NOT: return "NOT";
        case NODE_EXP: return "EXP";
        case NODE_ASSIGN: return "ASSIGN";
        case NODE_VAR_ASSIGN: return "VAR_ASSIGN";
        case NODE_FNCS: return "DECLFUNCTS";
        case NODE_FUNCTION_CALL: return "FUNCTION_CALL";
        case NODE_FUNCTION_HEADER: return "FUNCTION_HEADER";
        case NODE_ARGUMENTS: return "ARGUMENTS";
        case NODE_VARS: return "DECL_VARS";
        case NODE_FIELD_ACCESS : return "FIELD_ACCESS";
        case NODE_DECLSTRUCT : return "DECL_STRUCT";
        default: return "UNKNOWN";
    }
}

void printTree(Node *root, int indent) {
    static int rightmost[128]; // 1 = dernier fils, 0 = sinon

    if (!root) return;

    // Affichage des lignes verticales
    for (int i = 1; i < indent; i++) {
        printf(rightmost[i] ? "    " : "\u2502   ");
    }

    // Affichage des branches
    if (indent > 0) {
        printf(rightmost[indent] ? "\u2514\u2500\u2500 " : "\u251c\u2500\u2500 ");
    }

    // Affichage du label
    printf("%s", labelToString(root->label));

    switch(root->kind) {
        case KIND_INT: printf(" [%d]", root->intv); break;
        case KIND_CHAR: printf(" [%c]", root->charv); break;
        case KIND_IDENT: printf(" [%s]", root->identv); break;
        default: break;
    }

    printf("\n");

    // Parcours des enfants
    Node *child = root->firstChild;
    while (child) {
        rightmost[indent + 1] = (child->nextSibling == NULL);
        printTree(child, indent + 1);
        child = child->nextSibling;
    }
}
