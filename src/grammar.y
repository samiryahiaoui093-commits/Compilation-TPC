
%code requires {
    #include "ast.h"
}

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ast.h>
#define KIND_NONE   0
#define KIND_INT    1
#define KIND_CHAR   2
#define KIND_IDENT  3
int yylex();
void yyerror(const char *s);
extern int lines;
extern int column;

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

Node* newNode(label_t label, Node *firstChild, Node *nextSibling) {
    Node *node = (Node*)malloc(sizeof(Node));
    if(node == NULL) exit(1);
    node->intv = 0;
    node->charv = 0;
    node->identv = NULL;
    node->label = label;
    node->kind = KIND_NONE;
    node->firstChild = firstChild;
    node->nextSibling = nextSibling;
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
Node *root = NULL;

%}

%union {
    int intv;
    char charv;
    char* identv;
    Node* node;
}

%token <intv> NUM
%token <charv> CHARACTER ADDSUB DIVSTAR
%token <identv> IDENT TYPE EQ ORDER
%token IF ELSE WHILE RETURN VOID STRUCT AND OR  COMMENT

%type <node> Prog DeclVars Sdecl StructDecl Type DeclVstruct Declarateurs
%type <node> DeclFoncts DeclFonct EnTeteFonct Parametres ListTypVar Corps
%type <node> SuiteInstr Instr Rval Exp TB FB M E T F Arguments ListExp

%start Prog

%%

Prog:    DeclVars DeclFoncts {
                root = newNode(NODE_PROGRAM,$1,NULL);
                addChild(root,$2);
                $$=root;
}
    ;
DeclVars:
        { $$ = newNode(NODE_DECLVAR,NULL,NULL);}
    |   DeclVars Sdecl {
        addChild($1,$2);
        $$ = $1;
    }
    |   DeclVars StructDecl {
        addSibling($1,$2);
        $$ = $1;
    }

    ;

Sdecl:
    TYPE Declarateurs ';' {
        $$ = newNode(NODE_DECLVAR,NULL,NULL);
        Node *d = newNode(NODE_TYPE,NULL,NULL);
        d->identv = strdup($1);
        d->kind =KIND_IDENT;
        addChild($$,d);
        addSibling(d,$2);

    }
    | STRUCT IDENT Declarateurs ';' {
        Node *var = newNode(NODE_STRUCT,NULL,NULL);
        Node *d = newNode(NODE_IDENT,NULL,NULL);
        addChild(var,d);
        addSibling(d,$3);
        d->identv = $2;
        d->kind = 3;
        $$=var;

    }
    ;
StructDecl:
    STRUCT IDENT '{' DeclVstruct '}' ';' {
        Node *var = newNode(NODE_STRUCT,NULL,NULL);
        Node *second = newNode(NODE_IDENT,NULL,NULL);
        addChild(var,second);
        addSibling(second,$4);
        second->identv = strdup($2);
        second->kind = KIND_IDENT;
        $$ = var;

    }
Type:
    TYPE {
        $$ = newNode(NODE_TYPE, NULL, NULL);
        $$->identv = strdup($1);
    }
    | STRUCT IDENT {
        $$ = newNode(NODE_STRUCT, NULL, NULL);
        Node *second = newNode(NODE_IDENT, NULL, NULL);
        addChild($$, second);
        second->identv = strdup($2);
        second->kind = KIND_IDENT;
    }
;
DeclVstruct:
    DeclVstruct Type Declarateurs ';' {
        addChild($1, $2);
        addChild($1, $3);
        $$ = $1;
    }
  | Type Declarateurs ';' {
        Node *declNode = newNode(NODE_DECLVAR, NULL, NULL);
        addChild(declNode, $1);
        addChild(declNode, $2);
        $$ = declNode;
    }
;
Declarateurs:
       Declarateurs ',' IDENT {

        $$ = $1;
        Node *n = newNode(NODE_IDENT, NULL, NULL);
        if (n !=NULL){
            n->identv = strdup($3);
            n->kind = KIND_IDENT;
        }
        addSibling($$, n);
       }
    | IDENT {

        if( $1 != NULL){
            Node *n = newNode(NODE_IDENT, NULL, NULL);
            n->identv = strdup($1);
            n->kind = KIND_IDENT;
            $$ = n;
        } else {
            $$ = NULL;
        }


    }
;
DeclFoncts:
       DeclFoncts DeclFonct {
        $$ = newNode(NODE_FNCS,NULL,NULL);
        addSibling($$,$1);
        addSibling($$,$2);

       }
    |  DeclFonct {$$ = $1;}
    ;
DeclFonct:
       EnTeteFonct Corps{
       $$ = newNode(NODE_DECLFNCT,NULL,NULL);
        addChild($$,$1);
        addSibling($1,$2);
       }
    ;
EnTeteFonct:
    TYPE IDENT '(' Parametres ')' {
        $$ = newNode(NODE_DECLFNCT,NULL,NULL);
        Node *p = newNode(NODE_TYPE,NULL,NULL);
        p->kind =KIND_IDENT;
        p->identv = strdup($1);
        Node *i = newNode(NODE_IDENT,NULL,NULL);
        i->kind = KIND_IDENT;
        i->identv = strdup($2);
        addSibling(p,i);
        addChild($$,p);

    }
    |  VOID IDENT '(' Parametres ')'{
        $$ = newNode(NODE_DECLFNCT,NULL,NULL);
        Node *p = newNode(NODE_VOID,NULL,NULL);
        Node*ident = newNode(NODE_IDENT,NULL,NULL);
        addChild(p,ident);
        addChild($$,p);
        addChild($$,$4);
    }
    | STRUCT IDENT IDENT '(' Parametres ')'  {
        $$ = newNode(NODE_DECLFNCT,NULL,NULL);
        Node *p = newNode(NODE_IDENT,NULL,NULL);
        Node*ident = newNode(NODE_IDENT,NULL,NULL);
        Node* structure = newNode(NODE_STRUCT,NULL,NULL);
        addChild(structure,p);
        addChild($$,structure);
        addChild($$,ident);
        addChild($$,$5);
        structure->identv = $2;
        ident->identv = $3;


    }
    ;
Parametres:
       VOID {$$ = newNode(NODE_TYPE,NULL,NULL);}
    |  ListTypVar {$$ = $1;}
    ;
ListTypVar:
       ListTypVar ',' Type IDENT {
        $$ = newNode(NODE_DECLVAR,NULL,NULL);
        addChild($$,$1);
        addSibling($1,$3);
         Node * forth = newNode(NODE_IDENT,NULL,NULL);
        forth->identv =strdup($4);
        forth->kind = KIND_IDENT;
        addChild($3,forth);
       }
    |  Type IDENT {
        $$ = newNode(NODE_DECLVAR, NULL, NULL);
        addChild($$, $1);
        Node *ident = newNode(NODE_IDENT, NULL, NULL);
        ident->identv = strdup($2);
        ident->kind = KIND_IDENT;
        addChild($1, ident);

    }
    ;
Corps: '{' DeclVars SuiteInstr '}' {
    $$ = newNode(NODE_CORPS,NULL,NULL);
    addChild($$,$2);
    addChild($$,$3);
}
    ;
SuiteInstr:
       SuiteInstr Instr{
        if($1 == NULL){
            $$ = newNode(NODE_INSTR,NULL,NULL);
        } else {
            $$ = $1;
        }

        addChild($$,$2);
       }
    | {$$ = NULL;}
    ;


Instr:
       IDENT '=' Exp ';'  {

             Node * f = newNode(NODE_IDENT,NULL,NULL);
             $$= newNode(NODE_ASSIGN,NULL,NULL);
             f->identv =strdup($1);
             f-> kind = KIND_IDENT;
            addChild($$,f);
            addChild($$,$3);

       }
    |  IF '(' Exp ')' Instr {$$ = newNode(NODE_IF,NULL,NULL);
            addChild($$,$3);
            Node *instr = newNode(NODE_INSTR,NULL,NULL);
            addChild(instr,$5);
            addChild($$,instr);
    }
    |  IF '(' Exp ')' Instr ELSE Instr {$$ = newNode(NODE_IF,NULL,NULL);
            Node *elsenode = newNode(NODE_ELSE,NULL,NULL);
            addChild($$,$3);
            addChild($$,$5);
            addChild(elsenode,$7);
            addChild($$,elsenode);
    }
    |  WHILE '(' Exp ')' Instr {
        $$ = newNode(NODE_WHILE,NULL,NULL);
        addChild($$,$3);
        addChild($$,$5);
    }
    |  IDENT '(' Arguments  ')' ';'{
        $$ = newNode(NODE_FUNCTION,NULL,NULL);
        Node * second = newNode(NODE_IDENT,NULL,NULL);
        second->identv =strdup($1);
        second->kind = KIND_IDENT;
        addChild($$,second);
        addSibling(second,$3);
    }
    |  RETURN Rval ';'{
        $$ = newNode(NODE_RETURN,NULL,NULL);
        if($2) addChild($$,$2);
    }
    |  '{' SuiteInstr '}'{
        $$ = $2;
    }
    |  ';' {$$ = newNode(NODE_INSTR,NULL,NULL);}
     |{ $$ = newNode(NODE_INSTR,NULL,NULL);}
    ;
Rval:
    {$$=NULL;}
    |Exp {$$ = $1;}
    ;
Exp :  Exp OR TB {
        $$ = newNode(NODE_OR,NULL,NULL);
        addChild($$,$1);
        addChild($$,$3);

    }
    |  TB {$$ = $1;}
    ;
TB  :  TB AND FB {
        $$ = newNode(NODE_AND,NULL,NULL);
        addChild($$,$1);
        addChild($$,$3);
    }
    |  FB {$$ = $1;}
    ;
FB  :  FB EQ M {
        $$ = newNode(NODE_EQ,NULL,NULL);
        $$-> identv = strdup($2);
        addChild($$,$1);
        addChild($$,$3);
    }
    |  M {$$ = $1;}
    ;
M   :  M ORDER E {
        $$ = newNode(NODE_ORDER,NULL,NULL);
        $$->kind = KIND_IDENT;
        $$->identv = strdup($2);
        addChild($$,$1);
        addChild($$,$3);
    }
    |  E {$$ = $1;}
    ;
E   :  E ADDSUB T {
        $$ = newNode(NODE_ADDSUB,NULL,NULL);
        $$->kind = KIND_CHAR;
        $$->charv = $2;
        addChild($$,$1);
        addChild($$,$3);
    }
    |  T {$$ = $1;}
    ;
T   :  T DIVSTAR F {
        $$ = newNode(NODE_DIVSTAR,NULL,NULL);
        $$->kind = KIND_CHAR;
        $$->charv = $2;
        addChild($$,$1);
        addChild($$,$3);
    }
    |  F {$$ = $1;}
    ;
F   :  ADDSUB F {
        Node *f = newNode(NODE_ADDSUB,NULL,NULL);
        addChild(f,$2);
        $$ = f;
        }
    |  '!' F {
        Node *f = newNode(NODE_NOT,NULL,NULL);
        addChild(f,$2);
        $$ = f;
        }
    |  '(' Exp ')' {$$ = $2;}
    |  NUM {$$ = newNode(NODE_NUM,NULL,NULL);
            $$->intv = $1;
            $$->kind = KIND_INT;
            }
    |  CHARACTER {
        $$ = newNode(NODE_CHAR,NULL,NULL);
        $$->charv = $1;
        $$->kind = KIND_CHAR;
    }
    |  IDENT '(' Arguments  ')'{ $$ = newNode(NODE_IDENT,NULL,$3);
                                $$->identv =strdup($1);
                                }
    |  IDENT {$$ = newNode(NODE_IDENT,NULL,NULL);
                $$->kind = KIND_IDENT;
                $$->identv = strdup($1);
}
    ;

    ;
Arguments:
       ListExp {$$ = $1;}
    | {$$ = NULL;}
    ;
ListExp:
       ListExp ',' Exp {
                 $$ = $1;
                 addSibling($$,$3);
                        }
    |  Exp {$$ = $1;}
    ;
%%
int main(int argc,char ** argv) {
    int ret = yyparse();
    for(int i = 0 ; i < argc ; i ++){
        if(argc > 1 && (strcmp(argv[i],"-tree") == 0 || strcmp(argv[i],"-t") == 0 )){
            if (ret == 0 && root != NULL) {
                    printTree(root, 0);
                } else if (ret == 0 && root == NULL) {
                    fprintf(stderr, "Warning: parsing succeeded but AST root is NULL\n");
            }
    } else if( argc > 1 && (strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"-help") == 0 )){
        printf("options : -t or -tree will display the AST tree of the file parsed \n"
        "-h or -help will show the option available for the user.");
    }
    }



    return ret;
}

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n at line :%d  column :%d", s,lines,column);

}
