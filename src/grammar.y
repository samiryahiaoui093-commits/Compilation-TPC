
%code requires {
    #include "ast.h"

}

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ast.h>

int yylex();
void yyerror(const char *s);
extern int lines;
extern int column;


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
                root = newNode(NODE_PROGRAM);
                addChild(root,$1);
                addChild(root,$2);
                $$=root;
}
    ;
DeclVars:
        { $$ = newNode(NODE_DECLVAR);}
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
        $$ = newNode(NODE_DECLVAR);
        Node *d = newNode(NODE_TYPE);
        d->identv = strdup($1);
        d->kind =KIND_IDENT;
        addChild($$,d);
        addSibling(d,$2);

    }
    | STRUCT IDENT Declarateurs ';' {
        Node *var = newNode(NODE_STRUCT);
        var->kind= KIND_IDENT;
        var->identv =strdup($2);
        addSibling(var,$3);

        $$=var;

    }
    ;
StructDecl:
    STRUCT IDENT '{' DeclVstruct '}' ';' {
        Node *var = newNode(NODE_STRUCT);
        Node *second = newNode(NODE_IDENT);
        addChild(var,second);
        addSibling(second,$4);
        second->identv = strdup($2);
        second->kind = KIND_IDENT;
        $$ = var;

    }
Type:
    TYPE {
        $$ = newNode(NODE_TYPE);
        $$->kind = KIND_IDENT;
        $$->identv = strdup($1);
    }
    | STRUCT IDENT {
        $$ = newNode(NODE_STRUCT);
        Node *second = newNode(NODE_IDENT);
        addChild($$, second);
        second->identv = strdup($2);
        second->kind = KIND_IDENT;
    }
;
DeclVstruct:
    DeclVstruct Type Declarateurs ';' {
        addChild($1, $2);
        addChild($2, $3);
        $$ = $1;
    }
  | Type Declarateurs ';' {
        Node *declNode = newNode(NODE_DECLVAR);
        addChild(declNode, $1);
        addChild(declNode, $2);
        $$ = declNode;
    }
;
Declarateurs:
       Declarateurs ',' IDENT {

        $$ = $1;
        Node *n = newNode(NODE_IDENT);
        if (n !=NULL){
            n->identv = strdup($3);
            n->kind = KIND_IDENT;
        }
        addSibling($$, n);
       }
    | IDENT {

        if( $1 != NULL){
            Node *n = newNode(NODE_IDENT);
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
        $$ = $1;
        if($2 != NULL);
            addSibling($$,$2);
       }
    |  DeclFonct {$$ = $1;}
    ;
DeclFonct:
       EnTeteFonct Corps{
       $$ = newNode(NODE_DECLFNCT);
        addChild($$,$1);
        addSibling($1,$2);
       }
    ;
EnTeteFonct:
    TYPE IDENT '(' Parametres ')' {
        $$ = newNode(NODE_FUNCTION_HEADER);
        Node *p = newNode(NODE_TYPE);
        p->kind =KIND_IDENT;
        p->identv = strdup($1);
        Node *i = newNode(NODE_IDENT);
        i->kind = KIND_IDENT;
        i->identv = strdup($2);

        addSibling(p,i);
        addChild($$,p);
        addSibling(p,$4);
    }
    |  VOID IDENT '(' Parametres ')'{
        $$ = newNode(NODE_DECLFNCT);
        Node *p = newNode(NODE_VOID);
        p->kind =KIND_IDENT;
        p->identv = "void";
        Node *i = newNode(NODE_IDENT);
        i->kind = KIND_IDENT;
        i->identv = strdup($2);
        addChild(p,i);
        addChild($$,p);
        addChild($$,$4);
    }
    | STRUCT IDENT IDENT '(' Parametres ')'  {
        $$ = newNode(NODE_DECLFNCT);
        Node *p = newNode(NODE_IDENT);
        p->kind = KIND_IDENT;
        p->identv = strdup($2);
        Node*ident = newNode(NODE_IDENT);
        ident->kind = KIND_IDENT;
        ident->identv = strdup($3);
        Node* structure = newNode(NODE_STRUCT);
        addChild(structure,p);
        addChild($$,structure);
        addChild($$,ident);
        addChild($$,$5);
        structure->identv = $2;
        ident->identv = $3;


    }
    ;
Parametres:
       VOID {$$ = newNode(NODE_TYPE);
                $$->kind = KIND_IDENT;
                $$->identv = "void";
   }

    |  ListTypVar {$$ = $1;}
    ;
ListTypVar:
       ListTypVar ',' Type IDENT {

        $$ = $1;
        addChild($1,$3);
         Node * forth = newNode(NODE_IDENT);
        forth->identv =strdup($4);
        forth->kind = KIND_IDENT;
        addChild($3,forth);
       }
    |  Type IDENT {
        $$ = newNode(NODE_DECLVAR);
        addChild($$, $1);
        Node *ident = newNode(NODE_IDENT);
        ident->identv = strdup($2);
        ident->kind = KIND_IDENT;
        addSibling($1, ident);

    }
    ;
Corps: '{' DeclVars SuiteInstr '}' {
    $$ = newNode(NODE_CORPS);
    addChild($$,$2);
    addChild($$,$3);
}
    ;
SuiteInstr:
       SuiteInstr Instr{
        if($1 == NULL){
            $$ = newNode(NODE_INSTR);
        } else {
            $$ = $1;
        }

        addChild($$,$2);
       }
    | {$$ = NULL;}
    ;


Instr:
       IDENT '=' Exp ';'  {

             Node * f = newNode(NODE_IDENT);
             $$= newNode(NODE_ASSIGN);
             f->identv =strdup($1);
             f-> kind = KIND_IDENT;
            addChild($$,f);
            addChild($$,$3);

       }
    |  IF '(' Exp ')' Instr {$$ = newNode(NODE_IF);
            addChild($$,$3);
            Node *instr = newNode(NODE_INSTR);
            addChild(instr,$5);
            addChild($$,instr);
    }
    |  IF '(' Exp ')' Instr ELSE Instr {$$ = newNode(NODE_IF);
            Node *elsenode = newNode(NODE_ELSE);
            addChild($$,$3);
            addChild($$,$5);
            addChild(elsenode,$7);
            addChild($$,elsenode);
    }
    |  WHILE '(' Exp ')' Instr {
        $$ = newNode(NODE_WHILE);
        addChild($$,$3);
        addChild($$,$5);
    }
    |  IDENT '(' Arguments  ')' ';'{
        $$ = newNode(NODE_FUNCTION_CALL);
        Node * second = newNode(NODE_IDENT);
        second->identv =strdup($1);
        second->kind = KIND_IDENT;
        addChild($$,second);
        addSibling(second,$3);
    }
    |  RETURN Rval ';'{
        $$ = newNode(NODE_RETURN);
        if($2) addChild($$,$2);
    }
    |  '{' SuiteInstr '}'{
        $$ = $2;
    }
    |  ';' {$$ = newNode(NODE_INSTR);}
     |{ $$ = newNode(NODE_INSTR);}
    ;
Rval:
    {$$=NULL;}
    |Exp {$$ = $1;}
    ;
Exp :  Exp OR TB {
        $$ = newNode(NODE_OR);
        addChild($$,$1);
        addChild($$,$3);

    }
    |  TB {$$ = $1;}
    ;
TB  :  TB AND FB {
        $$ = newNode(NODE_AND);
        addChild($$,$1);
        addChild($$,$3);
    }
    |  FB {$$ = $1;}
    ;
FB  :  FB EQ M {
        $$ = newNode(NODE_EQ);
        $$-> identv = strdup($2);
        addChild($$,$1);
        addChild($$,$3);
    }
    |  M {$$ = $1;}
    ;
M   :  M ORDER E {
        $$ = newNode(NODE_ORDER);
        $$->kind = KIND_IDENT;
        $$->identv = strdup($2);
        addChild($$,$1);
        addChild($$,$3);
    }
    |  E {$$ = $1;}
    ;
E   :  E ADDSUB T {
        $$ = newNode(NODE_ADDSUB);
        $$->kind = KIND_CHAR;
        $$->charv = $2;
        addChild($$,$1);
        addChild($$,$3);
    }
    |  T {$$ = $1;}
    ;
T   :  T DIVSTAR F {
        $$ = newNode(NODE_DIVSTAR);
        $$->kind = KIND_CHAR;
        $$->charv = $2;
        addChild($$,$1);
        addChild($$,$3);
    }
    |  F {$$ = $1;}
    ;
F   :  ADDSUB F {
        Node *f = newNode(NODE_ADDSUB);
        addChild(f,$2);
        $$ = f;
        }
    |  '!' F {
        Node *f = newNode(NODE_NOT);
        addChild(f,$2);
        $$ = f;
        }
    |  '(' Exp ')' {$$ = $2;}
    |  NUM {$$ = newNode(NODE_NUM);
            $$->intv = $1;
            $$->kind = KIND_INT;
            }
    |  CHARACTER {
        $$ = newNode(NODE_CHAR);
        $$->charv = $1;
        $$->kind = KIND_CHAR;
    }
    |  IDENT '(' Arguments  ')'{
        $$ = newNode(NODE_FUNCTION_CALL);
        Node * second = newNode(NODE_IDENT);
        second->identv =strdup($1);
        second->kind = KIND_IDENT;
        addChild($$,second);
        addSibling(second,$3);

         }
    |  IDENT {$$ = newNode(NODE_IDENT);
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
