#ifndef AST_H
#define AST_H
#define KIND_NONE   0
#define KIND_INT    1
#define KIND_CHAR   2
#define KIND_IDENT  3
typedef enum {
    NODE_PROGRAM, NODE_NUM, NODE_IDENT, NODE_ADDSUB, NODE_DIVSTAR,
    NODE_VOID, NODE_IF, NODE_ELSE, NODE_RETURN, NODE_AND, NODE_OR,
    NODE_EQ, NODE_ORDER, NODE_TYPE, NODE_DECLVAR, NODE_DECLVARSTRUCT,
    NODE_ENTETE, NODE_PARAMETRE, NODE_CORPS, NODE_INSTR, NODE_DECLFNCT,
    NODE_STRUCT, NODE_CHAR, NODE_WHILE, NODE_FUNCTION, NODE_NOT,
    NODE_EXP, NODE_ASSIGN, NODE_VAR_ASSIGN,NODE_FNCS,NODE_FUNCTION_CALL,NODE_FUNCTION_HEADER,NODE_ARGUMENTS
} label_t;

typedef struct Node {
    label_t label;
    int kind;
    int line,column;
    int argument_number;
    union {
        int intv;
        char charv;
        char* identv;
    };
    struct Node *firstChild, *nextSibling;
} Node;

#endif



const char* labelToString(label_t label);

void addSibling(Node *node, Node *nextSibling);

void addChild(Node *node, Node *nextChild);

Node* newNode(label_t label);

const char* labelToString(label_t label);

void printTree(Node *root, int indent);
