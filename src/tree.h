/* tree.h */


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
  struct Node *firstChild, *nextSibling;
  int lineno;
} Node;

Node *makeNode(label_t label);
void addSibling(Node *node, Node *sibling);
void addChild(Node *parent, Node *child);
void deleteTree(Node*node);
void printTree(Node *node);

#define FIRSTCHILD(node) node->firstChild
#define SECONDCHILD(node) node->firstChild->nextSibling
#define THIRDCHILD(node) node->firstChild->nextSibling->nextSibling
