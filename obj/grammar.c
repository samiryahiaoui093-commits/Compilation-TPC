/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 6 "src/grammar.y"


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
        case NODE_FNCS: return "DECLFUNCTS";
        case NODE_FUNCTION_CALL: return "FUNCTION_CALL";
        case NODE_FUNCTION_HEADER: return "FUNCTION_HEADER";
        case NODE_ARGUMENTS: return "ARGUMENTS";
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


#line 194 "obj/grammar.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "grammar.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUM = 3,                        /* NUM  */
  YYSYMBOL_CHARACTER = 4,                  /* CHARACTER  */
  YYSYMBOL_ADDSUB = 5,                     /* ADDSUB  */
  YYSYMBOL_DIVSTAR = 6,                    /* DIVSTAR  */
  YYSYMBOL_IDENT = 7,                      /* IDENT  */
  YYSYMBOL_TYPE = 8,                       /* TYPE  */
  YYSYMBOL_EQ = 9,                         /* EQ  */
  YYSYMBOL_ORDER = 10,                     /* ORDER  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_WHILE = 13,                     /* WHILE  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_VOID = 15,                      /* VOID  */
  YYSYMBOL_STRUCT = 16,                    /* STRUCT  */
  YYSYMBOL_AND = 17,                       /* AND  */
  YYSYMBOL_OR = 18,                        /* OR  */
  YYSYMBOL_COMMENT = 19,                   /* COMMENT  */
  YYSYMBOL_20_ = 20,                       /* ';'  */
  YYSYMBOL_21_ = 21,                       /* '{'  */
  YYSYMBOL_22_ = 22,                       /* '}'  */
  YYSYMBOL_23_ = 23,                       /* ','  */
  YYSYMBOL_24_ = 24,                       /* '('  */
  YYSYMBOL_25_ = 25,                       /* ')'  */
  YYSYMBOL_26_ = 26,                       /* '='  */
  YYSYMBOL_27_ = 27,                       /* '!'  */
  YYSYMBOL_YYACCEPT = 28,                  /* $accept  */
  YYSYMBOL_Prog = 29,                      /* Prog  */
  YYSYMBOL_DeclVars = 30,                  /* DeclVars  */
  YYSYMBOL_Sdecl = 31,                     /* Sdecl  */
  YYSYMBOL_StructDecl = 32,                /* StructDecl  */
  YYSYMBOL_Type = 33,                      /* Type  */
  YYSYMBOL_DeclVstruct = 34,               /* DeclVstruct  */
  YYSYMBOL_Declarateurs = 35,              /* Declarateurs  */
  YYSYMBOL_DeclFoncts = 36,                /* DeclFoncts  */
  YYSYMBOL_DeclFonct = 37,                 /* DeclFonct  */
  YYSYMBOL_EnTeteFonct = 38,               /* EnTeteFonct  */
  YYSYMBOL_Parametres = 39,                /* Parametres  */
  YYSYMBOL_ListTypVar = 40,                /* ListTypVar  */
  YYSYMBOL_Corps = 41,                     /* Corps  */
  YYSYMBOL_SuiteInstr = 42,                /* SuiteInstr  */
  YYSYMBOL_Instr = 43,                     /* Instr  */
  YYSYMBOL_Rval = 44,                      /* Rval  */
  YYSYMBOL_Exp = 45,                       /* Exp  */
  YYSYMBOL_TB = 46,                        /* TB  */
  YYSYMBOL_FB = 47,                        /* FB  */
  YYSYMBOL_M = 48,                         /* M  */
  YYSYMBOL_E = 49,                         /* E  */
  YYSYMBOL_T = 50,                         /* T  */
  YYSYMBOL_F = 51,                         /* F  */
  YYSYMBOL_Arguments = 52,                 /* Arguments  */
  YYSYMBOL_ListExp = 53                    /* ListExp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   131

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  130

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   274


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,     2,     2,     2,     2,     2,
      24,    25,     2,     2,    23,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    20,
       2,    26,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    21,     2,    22,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   149,   149,   156,   157,   161,   169,   178,   190,   201,
     206,   215,   220,   228,   238,   253,   258,   261,   268,   281,
     293,   313,   318,   321,   330,   340,   347,   356,   361,   371,
     377,   384,   389,   397,   401,   404,   405,   408,   409,   411,
     417,   419,   424,   426,   432,   434,   441,   443,   450,   452,
     459,   461,   466,   471,   472,   476,   481,   490,   498,   499,
     502,   506
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "CHARACTER",
  "ADDSUB", "DIVSTAR", "IDENT", "TYPE", "EQ", "ORDER", "IF", "ELSE",
  "WHILE", "RETURN", "VOID", "STRUCT", "AND", "OR", "COMMENT", "';'",
  "'{'", "'}'", "','", "'('", "')'", "'='", "'!'", "$accept", "Prog",
  "DeclVars", "Sdecl", "StructDecl", "Type", "DeclVstruct", "Declarateurs",
  "DeclFoncts", "DeclFonct", "EnTeteFonct", "Parametres", "ListTypVar",
  "Corps", "SuiteInstr", "Instr", "Rval", "Exp", "TB", "FB", "M", "E", "T",
  "F", "Arguments", "ListExp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-94)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -94,    51,    53,   -94,    72,    73,    74,   -94,   -94,    55,
     -94,    54,    62,    14,    65,    -4,    85,    86,   -94,   -94,
     -94,    57,   -94,    87,    57,    71,     7,    26,    62,    89,
      66,   -94,   -94,    90,    91,    75,    76,   -94,    77,    57,
      94,    36,   -94,    71,    94,    96,    18,   -94,   -94,   -94,
       7,   -94,    79,   -94,    67,    88,    94,    15,   -10,    81,
      82,     3,   -94,   -94,   -94,   -94,   100,   -94,   -94,   -94,
      68,     3,     3,     3,     3,   -94,   -94,     3,    92,     3,
       3,    93,    97,    95,   101,    99,   106,   108,   -94,    34,
     -94,   -94,    97,    98,   102,    44,    58,    59,   -94,     3,
      60,   -94,   -94,     3,     3,     3,     3,     3,     3,   -94,
     104,     3,   -94,    46,    46,   103,   -94,    95,   101,    99,
     106,   108,   -94,   -94,    97,   105,   -94,   -94,    46,   -94
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     1,     0,     0,     0,     4,     5,     2,
      16,     0,    14,     0,     0,     0,     0,     0,    15,     3,
      17,     0,     6,     0,     0,    14,     0,     0,     0,     0,
      27,     9,    21,     0,     0,     0,    22,    13,     0,     0,
       0,     0,     7,     0,     0,     0,     0,    10,    24,    18,
       0,    19,     0,    14,     0,     0,     0,     0,     0,     0,
       0,    37,    35,    27,    25,    26,     0,    20,    12,     8,
       0,    59,     0,     0,     0,    54,    55,     0,    57,     0,
       0,     0,    38,    40,    42,    44,    46,    48,    50,     0,
      23,    11,    61,     0,    58,     0,     0,     0,    51,    59,
       0,    52,    33,     0,     0,     0,     0,     0,     0,    34,
       0,     0,    28,    36,    36,     0,    53,    39,    41,    43,
      45,    47,    49,    32,    60,    29,    31,    56,    36,    30
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -94,   -94,   107,   -94,   -94,   -22,   -94,   -14,   -94,   109,
     -94,   -15,   -94,   -94,    56,   -93,   -94,   -61,    17,    23,
      16,    24,    22,   -75,    32,   -94
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     7,     8,    34,    41,    13,     9,    10,
      11,    35,    36,    20,    46,    65,    81,    92,    83,    84,
      85,    86,    87,    88,    93,    94
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      82,    27,    98,    25,    40,   101,    75,    76,    77,    38,
      78,    95,    96,    97,    71,    31,    72,    26,   100,    56,
     125,   126,    53,    33,    52,    58,    54,    79,    66,    59,
      80,    60,    61,   122,    22,   129,    26,    23,    62,    63,
      64,    58,    70,    27,    31,    59,    42,    60,    61,    23,
     124,     3,    33,    58,    62,    63,   109,    59,    55,    60,
      61,     4,   103,    16,   112,    31,    62,    63,     5,     6,
       5,    17,    32,    33,    44,    19,   103,   103,   103,    12,
      14,    15,    45,   113,   114,   116,    21,    68,    91,    24,
      23,    23,    28,    29,    37,    39,    43,    47,    48,    50,
      49,    53,    51,    57,    67,    73,    74,    90,    69,   106,
     105,   107,   104,   102,   108,   103,    99,   128,    18,    89,
     117,   119,     0,   110,   123,   111,    30,   118,   127,   121,
     120,   115
};

static const yytype_int16 yycheck[] =
{
      61,    15,    77,     7,    26,    80,     3,     4,     5,    24,
       7,    72,    73,    74,    24,     8,    26,    21,    79,    41,
     113,   114,     7,    16,    39,     7,    40,    24,    50,    11,
      27,    13,    14,   108,    20,   128,    21,    23,    20,    21,
      22,     7,    56,    57,     8,    11,    20,    13,    14,    23,
     111,     0,    16,     7,    20,    21,    22,    11,    22,    13,
      14,     8,    18,     8,    20,     8,    20,    21,    15,    16,
      15,    16,    15,    16,     8,    21,    18,    18,    18,     7,
       7,     7,    16,    25,    25,    25,    24,    20,    20,    24,
      23,    23,     7,     7,     7,    24,     7,     7,     7,    23,
      25,     7,    25,     7,    25,    24,    24,     7,    20,    10,
       9,     5,    17,    20,     6,    18,    24,    12,     9,    63,
     103,   105,    -1,    25,    20,    23,    19,   104,    25,   107,
     106,    99
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    29,    30,     0,     8,    15,    16,    31,    32,    36,
      37,    38,     7,    35,     7,     7,     8,    16,    37,    21,
      41,    24,    20,    23,    24,     7,    21,    35,     7,     7,
      30,     8,    15,    16,    33,    39,    40,     7,    39,    24,
      33,    34,    20,     7,     8,    16,    42,     7,     7,    25,
      23,    25,    39,     7,    35,    22,    33,     7,     7,    11,
      13,    14,    20,    21,    22,    43,    33,    25,    20,    20,
      35,    24,    26,    24,    24,     3,     4,     5,     7,    24,
      27,    44,    45,    46,    47,    48,    49,    50,    51,    42,
       7,    20,    45,    52,    53,    45,    45,    45,    51,    24,
      45,    51,    20,    18,    17,     9,    10,     5,     6,    22,
      25,    23,    20,    25,    25,    52,    25,    46,    47,    48,
      49,    50,    51,    20,    45,    43,    43,    25,    12,    43
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    28,    29,    30,    30,    30,    31,    31,    32,    33,
      33,    34,    34,    35,    35,    36,    36,    37,    38,    38,
      38,    39,    39,    40,    40,    41,    42,    42,    43,    43,
      43,    43,    43,    43,    43,    43,    43,    44,    44,    45,
      45,    46,    46,    47,    47,    48,    48,    49,    49,    50,
      50,    51,    51,    51,    51,    51,    51,    51,    52,    52,
      53,    53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     2,     2,     3,     4,     6,     1,
       2,     4,     3,     3,     1,     2,     1,     2,     5,     5,
       6,     1,     1,     4,     2,     4,     2,     0,     4,     5,
       7,     5,     5,     3,     3,     1,     0,     0,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     2,     2,     3,     1,     1,     4,     1,     1,     0,
       3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Prog: DeclVars DeclFoncts  */
#line 149 "src/grammar.y"
                             {
                root = newNode(NODE_PROGRAM,(yyvsp[-1].node),NULL);
                addChild(root,(yyvsp[0].node));
                (yyval.node)=root;
}
#line 1316 "obj/grammar.c"
    break;

  case 3: /* DeclVars: %empty  */
#line 156 "src/grammar.y"
        { (yyval.node) = newNode(NODE_DECLVAR,NULL,NULL);}
#line 1322 "obj/grammar.c"
    break;

  case 4: /* DeclVars: DeclVars Sdecl  */
#line 157 "src/grammar.y"
                       {
        addChild((yyvsp[-1].node),(yyvsp[0].node));
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1331 "obj/grammar.c"
    break;

  case 5: /* DeclVars: DeclVars StructDecl  */
#line 161 "src/grammar.y"
                            {
        addSibling((yyvsp[-1].node),(yyvsp[0].node));
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1340 "obj/grammar.c"
    break;

  case 6: /* Sdecl: TYPE Declarateurs ';'  */
#line 169 "src/grammar.y"
                          {
        (yyval.node) = newNode(NODE_DECLVAR,NULL,NULL);
        Node *d = newNode(NODE_TYPE,NULL,NULL);
        d->identv = strdup((yyvsp[-2].identv));
        d->kind =KIND_IDENT;
        addChild((yyval.node),d);
        addSibling(d,(yyvsp[-1].node));

    }
#line 1354 "obj/grammar.c"
    break;

  case 7: /* Sdecl: STRUCT IDENT Declarateurs ';'  */
#line 178 "src/grammar.y"
                                    {
        Node *var = newNode(NODE_STRUCT,NULL,NULL);
        Node *d = newNode(NODE_IDENT,NULL,NULL);
        addChild(var,d);
        addSibling(d,(yyvsp[-1].node));
        d->identv = (yyvsp[-2].identv);
        d->kind = 3;
        (yyval.node)=var;

    }
#line 1369 "obj/grammar.c"
    break;

  case 8: /* StructDecl: STRUCT IDENT '{' DeclVstruct '}' ';'  */
#line 190 "src/grammar.y"
                                         {
        Node *var = newNode(NODE_STRUCT,NULL,NULL);
        Node *second = newNode(NODE_IDENT,NULL,NULL);
        addChild(var,second);
        addSibling(second,(yyvsp[-2].node));
        second->identv = strdup((yyvsp[-4].identv));
        second->kind = KIND_IDENT;
        (yyval.node) = var;

    }
#line 1384 "obj/grammar.c"
    break;

  case 9: /* Type: TYPE  */
#line 201 "src/grammar.y"
         {
        (yyval.node) = newNode(NODE_TYPE, NULL, NULL);
        (yyval.node)->kind = KIND_IDENT;
        (yyval.node)->identv = strdup((yyvsp[0].identv));
    }
#line 1394 "obj/grammar.c"
    break;

  case 10: /* Type: STRUCT IDENT  */
#line 206 "src/grammar.y"
                   {
        (yyval.node) = newNode(NODE_STRUCT, NULL, NULL);
        Node *second = newNode(NODE_IDENT, NULL, NULL);
        addChild((yyval.node), second);
        second->identv = strdup((yyvsp[0].identv));
        second->kind = KIND_IDENT;
    }
#line 1406 "obj/grammar.c"
    break;

  case 11: /* DeclVstruct: DeclVstruct Type Declarateurs ';'  */
#line 215 "src/grammar.y"
                                      {
        addChild((yyvsp[-3].node), (yyvsp[-2].node));
        addChild((yyvsp[-2].node), (yyvsp[-1].node));
        (yyval.node) = (yyvsp[-3].node);
    }
#line 1416 "obj/grammar.c"
    break;

  case 12: /* DeclVstruct: Type Declarateurs ';'  */
#line 220 "src/grammar.y"
                          {
        Node *declNode = newNode(NODE_DECLVAR, NULL, NULL);
        addChild(declNode, (yyvsp[-2].node));
        addChild(declNode, (yyvsp[-1].node));
        (yyval.node) = declNode;
    }
#line 1427 "obj/grammar.c"
    break;

  case 13: /* Declarateurs: Declarateurs ',' IDENT  */
#line 228 "src/grammar.y"
                              {

        (yyval.node) = (yyvsp[-2].node);
        Node *n = newNode(NODE_IDENT, NULL, NULL);
        if (n !=NULL){
            n->identv = strdup((yyvsp[0].identv));
            n->kind = KIND_IDENT;
        }
        addSibling((yyval.node), n);
       }
#line 1442 "obj/grammar.c"
    break;

  case 14: /* Declarateurs: IDENT  */
#line 238 "src/grammar.y"
            {

        if( (yyvsp[0].identv) != NULL){
            Node *n = newNode(NODE_IDENT, NULL, NULL);
            n->identv = strdup((yyvsp[0].identv));
            n->kind = KIND_IDENT;
            (yyval.node) = n;
        } else {
            (yyval.node) = NULL;
        }


    }
#line 1460 "obj/grammar.c"
    break;

  case 15: /* DeclFoncts: DeclFoncts DeclFonct  */
#line 253 "src/grammar.y"
                            {
        (yyval.node) = (yyvsp[-1].node);
        if((yyvsp[0].node) != NULL);
            addSibling((yyval.node),(yyvsp[0].node));
       }
#line 1470 "obj/grammar.c"
    break;

  case 16: /* DeclFoncts: DeclFonct  */
#line 258 "src/grammar.y"
                 {(yyval.node) = (yyvsp[0].node);}
#line 1476 "obj/grammar.c"
    break;

  case 17: /* DeclFonct: EnTeteFonct Corps  */
#line 261 "src/grammar.y"
                        {
       (yyval.node) = newNode(NODE_DECLFNCT,NULL,NULL);
        addChild((yyval.node),(yyvsp[-1].node));
        addSibling((yyvsp[-1].node),(yyvsp[0].node));
       }
#line 1486 "obj/grammar.c"
    break;

  case 18: /* EnTeteFonct: TYPE IDENT '(' Parametres ')'  */
#line 268 "src/grammar.y"
                                  {
        (yyval.node) = newNode(NODE_FUNCTION_HEADER,NULL,NULL);
        Node *p = newNode(NODE_TYPE,NULL,NULL);
        p->kind =KIND_IDENT;
        p->identv = strdup((yyvsp[-4].identv));
        Node *i = newNode(NODE_IDENT,NULL,NULL);
        i->kind = KIND_IDENT;
        i->identv = strdup((yyvsp[-3].identv));

        addSibling(p,i);
        addChild((yyval.node),p);
        addSibling(p,(yyvsp[-1].node));
    }
#line 1504 "obj/grammar.c"
    break;

  case 19: /* EnTeteFonct: VOID IDENT '(' Parametres ')'  */
#line 281 "src/grammar.y"
                                    {
        (yyval.node) = newNode(NODE_DECLFNCT,NULL,NULL);
        Node *p = newNode(NODE_VOID,NULL,NULL);
        p->kind =KIND_IDENT;
        p->identv = "void";
        Node *i = newNode(NODE_IDENT,NULL,NULL);
        i->kind = KIND_IDENT;
        i->identv = strdup((yyvsp[-3].identv));
        addChild(p,i);
        addChild((yyval.node),p);
        addChild((yyval.node),(yyvsp[-1].node));
    }
#line 1521 "obj/grammar.c"
    break;

  case 20: /* EnTeteFonct: STRUCT IDENT IDENT '(' Parametres ')'  */
#line 293 "src/grammar.y"
                                             {
        (yyval.node) = newNode(NODE_DECLFNCT,NULL,NULL);
        Node *p = newNode(NODE_IDENT,NULL,NULL);
        p->kind = KIND_IDENT;
        p->identv = strdup((yyvsp[-4].identv));
        Node*ident = newNode(NODE_IDENT,NULL,NULL);
        ident->kind = KIND_IDENT;
        ident->identv = strdup((yyvsp[-3].identv));
        Node* structure = newNode(NODE_STRUCT,NULL,NULL);
        addChild(structure,p);
        addChild((yyval.node),structure);
        addChild((yyval.node),ident);
        addChild((yyval.node),(yyvsp[-1].node));
        structure->identv = (yyvsp[-4].identv);
        ident->identv = (yyvsp[-3].identv);


    }
#line 1544 "obj/grammar.c"
    break;

  case 21: /* Parametres: VOID  */
#line 313 "src/grammar.y"
            {(yyval.node) = newNode(NODE_TYPE,NULL,NULL);
                (yyval.node)->kind = KIND_IDENT;
                (yyval.node)->identv = "void";
   }
#line 1553 "obj/grammar.c"
    break;

  case 22: /* Parametres: ListTypVar  */
#line 318 "src/grammar.y"
                  {(yyval.node) = (yyvsp[0].node);}
#line 1559 "obj/grammar.c"
    break;

  case 23: /* ListTypVar: ListTypVar ',' Type IDENT  */
#line 321 "src/grammar.y"
                                 {

        (yyval.node) = (yyvsp[-3].node);
        addChild((yyvsp[-3].node),(yyvsp[-1].node));
         Node * forth = newNode(NODE_IDENT,NULL,NULL);
        forth->identv =strdup((yyvsp[0].identv));
        forth->kind = KIND_IDENT;
        addChild((yyvsp[-1].node),forth);
       }
#line 1573 "obj/grammar.c"
    break;

  case 24: /* ListTypVar: Type IDENT  */
#line 330 "src/grammar.y"
                  {
        (yyval.node) = newNode(NODE_DECLVAR, NULL, NULL);
        addChild((yyval.node), (yyvsp[-1].node));
        Node *ident = newNode(NODE_IDENT, NULL, NULL);
        ident->identv = strdup((yyvsp[0].identv));
        ident->kind = KIND_IDENT;
        addSibling((yyvsp[-1].node), ident);

    }
#line 1587 "obj/grammar.c"
    break;

  case 25: /* Corps: '{' DeclVars SuiteInstr '}'  */
#line 340 "src/grammar.y"
                                   {
    (yyval.node) = newNode(NODE_CORPS,NULL,NULL);
    addChild((yyval.node),(yyvsp[-2].node));
    addChild((yyval.node),(yyvsp[-1].node));
}
#line 1597 "obj/grammar.c"
    break;

  case 26: /* SuiteInstr: SuiteInstr Instr  */
#line 347 "src/grammar.y"
                       {
        if((yyvsp[-1].node) == NULL){
            (yyval.node) = newNode(NODE_INSTR,NULL,NULL);
        } else {
            (yyval.node) = (yyvsp[-1].node);
        }

        addChild((yyval.node),(yyvsp[0].node));
       }
#line 1611 "obj/grammar.c"
    break;

  case 27: /* SuiteInstr: %empty  */
#line 356 "src/grammar.y"
      {(yyval.node) = NULL;}
#line 1617 "obj/grammar.c"
    break;

  case 28: /* Instr: IDENT '=' Exp ';'  */
#line 361 "src/grammar.y"
                          {

             Node * f = newNode(NODE_IDENT,NULL,NULL);
             (yyval.node)= newNode(NODE_ASSIGN,NULL,NULL);
             f->identv =strdup((yyvsp[-3].identv));
             f-> kind = KIND_IDENT;
            addChild((yyval.node),f);
            addChild((yyval.node),(yyvsp[-1].node));

       }
#line 1632 "obj/grammar.c"
    break;

  case 29: /* Instr: IF '(' Exp ')' Instr  */
#line 371 "src/grammar.y"
                            {(yyval.node) = newNode(NODE_IF,NULL,NULL);
            addChild((yyval.node),(yyvsp[-2].node));
            Node *instr = newNode(NODE_INSTR,NULL,NULL);
            addChild(instr,(yyvsp[0].node));
            addChild((yyval.node),instr);
    }
#line 1643 "obj/grammar.c"
    break;

  case 30: /* Instr: IF '(' Exp ')' Instr ELSE Instr  */
#line 377 "src/grammar.y"
                                       {(yyval.node) = newNode(NODE_IF,NULL,NULL);
            Node *elsenode = newNode(NODE_ELSE,NULL,NULL);
            addChild((yyval.node),(yyvsp[-4].node));
            addChild((yyval.node),(yyvsp[-2].node));
            addChild(elsenode,(yyvsp[0].node));
            addChild((yyval.node),elsenode);
    }
#line 1655 "obj/grammar.c"
    break;

  case 31: /* Instr: WHILE '(' Exp ')' Instr  */
#line 384 "src/grammar.y"
                               {
        (yyval.node) = newNode(NODE_WHILE,NULL,NULL);
        addChild((yyval.node),(yyvsp[-2].node));
        addChild((yyval.node),(yyvsp[0].node));
    }
#line 1665 "obj/grammar.c"
    break;

  case 32: /* Instr: IDENT '(' Arguments ')' ';'  */
#line 389 "src/grammar.y"
                                   {
        (yyval.node) = newNode(NODE_FUNCTION_CALL,NULL,NULL);
        Node * second = newNode(NODE_IDENT,NULL,NULL);
        second->identv =strdup((yyvsp[-4].identv));
        second->kind = KIND_IDENT;
        addChild((yyval.node),second);
        addSibling(second,(yyvsp[-2].node));
    }
#line 1678 "obj/grammar.c"
    break;

  case 33: /* Instr: RETURN Rval ';'  */
#line 397 "src/grammar.y"
                      {
        (yyval.node) = newNode(NODE_RETURN,NULL,NULL);
        if((yyvsp[-1].node)) addChild((yyval.node),(yyvsp[-1].node));
    }
#line 1687 "obj/grammar.c"
    break;

  case 34: /* Instr: '{' SuiteInstr '}'  */
#line 401 "src/grammar.y"
                         {
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1695 "obj/grammar.c"
    break;

  case 35: /* Instr: ';'  */
#line 404 "src/grammar.y"
           {(yyval.node) = newNode(NODE_INSTR,NULL,NULL);}
#line 1701 "obj/grammar.c"
    break;

  case 36: /* Instr: %empty  */
#line 405 "src/grammar.y"
      { (yyval.node) = newNode(NODE_INSTR,NULL,NULL);}
#line 1707 "obj/grammar.c"
    break;

  case 37: /* Rval: %empty  */
#line 408 "src/grammar.y"
    {(yyval.node)=NULL;}
#line 1713 "obj/grammar.c"
    break;

  case 38: /* Rval: Exp  */
#line 409 "src/grammar.y"
         {(yyval.node) = (yyvsp[0].node);}
#line 1719 "obj/grammar.c"
    break;

  case 39: /* Exp: Exp OR TB  */
#line 411 "src/grammar.y"
                 {
        (yyval.node) = newNode(NODE_OR,NULL,NULL);
        addChild((yyval.node),(yyvsp[-2].node));
        addChild((yyval.node),(yyvsp[0].node));

    }
#line 1730 "obj/grammar.c"
    break;

  case 40: /* Exp: TB  */
#line 417 "src/grammar.y"
          {(yyval.node) = (yyvsp[0].node);}
#line 1736 "obj/grammar.c"
    break;

  case 41: /* TB: TB AND FB  */
#line 419 "src/grammar.y"
                 {
        (yyval.node) = newNode(NODE_AND,NULL,NULL);
        addChild((yyval.node),(yyvsp[-2].node));
        addChild((yyval.node),(yyvsp[0].node));
    }
#line 1746 "obj/grammar.c"
    break;

  case 42: /* TB: FB  */
#line 424 "src/grammar.y"
          {(yyval.node) = (yyvsp[0].node);}
#line 1752 "obj/grammar.c"
    break;

  case 43: /* FB: FB EQ M  */
#line 426 "src/grammar.y"
               {
        (yyval.node) = newNode(NODE_EQ,NULL,NULL);
        (yyval.node)-> identv = strdup((yyvsp[-1].identv));
        addChild((yyval.node),(yyvsp[-2].node));
        addChild((yyval.node),(yyvsp[0].node));
    }
#line 1763 "obj/grammar.c"
    break;

  case 44: /* FB: M  */
#line 432 "src/grammar.y"
         {(yyval.node) = (yyvsp[0].node);}
#line 1769 "obj/grammar.c"
    break;

  case 45: /* M: M ORDER E  */
#line 434 "src/grammar.y"
                 {
        (yyval.node) = newNode(NODE_ORDER,NULL,NULL);
        (yyval.node)->kind = KIND_IDENT;
        (yyval.node)->identv = strdup((yyvsp[-1].identv));
        addChild((yyval.node),(yyvsp[-2].node));
        addChild((yyval.node),(yyvsp[0].node));
    }
#line 1781 "obj/grammar.c"
    break;

  case 46: /* M: E  */
#line 441 "src/grammar.y"
         {(yyval.node) = (yyvsp[0].node);}
#line 1787 "obj/grammar.c"
    break;

  case 47: /* E: E ADDSUB T  */
#line 443 "src/grammar.y"
                  {
        (yyval.node) = newNode(NODE_ADDSUB,NULL,NULL);
        (yyval.node)->kind = KIND_CHAR;
        (yyval.node)->charv = (yyvsp[-1].charv);
        addChild((yyval.node),(yyvsp[-2].node));
        addChild((yyval.node),(yyvsp[0].node));
    }
#line 1799 "obj/grammar.c"
    break;

  case 48: /* E: T  */
#line 450 "src/grammar.y"
         {(yyval.node) = (yyvsp[0].node);}
#line 1805 "obj/grammar.c"
    break;

  case 49: /* T: T DIVSTAR F  */
#line 452 "src/grammar.y"
                   {
        (yyval.node) = newNode(NODE_DIVSTAR,NULL,NULL);
        (yyval.node)->kind = KIND_CHAR;
        (yyval.node)->charv = (yyvsp[-1].charv);
        addChild((yyval.node),(yyvsp[-2].node));
        addChild((yyval.node),(yyvsp[0].node));
    }
#line 1817 "obj/grammar.c"
    break;

  case 50: /* T: F  */
#line 459 "src/grammar.y"
         {(yyval.node) = (yyvsp[0].node);}
#line 1823 "obj/grammar.c"
    break;

  case 51: /* F: ADDSUB F  */
#line 461 "src/grammar.y"
                {
        Node *f = newNode(NODE_ADDSUB,NULL,NULL);
        addChild(f,(yyvsp[0].node));
        (yyval.node) = f;
        }
#line 1833 "obj/grammar.c"
    break;

  case 52: /* F: '!' F  */
#line 466 "src/grammar.y"
             {
        Node *f = newNode(NODE_NOT,NULL,NULL);
        addChild(f,(yyvsp[0].node));
        (yyval.node) = f;
        }
#line 1843 "obj/grammar.c"
    break;

  case 53: /* F: '(' Exp ')'  */
#line 471 "src/grammar.y"
                   {(yyval.node) = (yyvsp[-1].node);}
#line 1849 "obj/grammar.c"
    break;

  case 54: /* F: NUM  */
#line 472 "src/grammar.y"
           {(yyval.node) = newNode(NODE_NUM,NULL,NULL);
            (yyval.node)->intv = (yyvsp[0].intv);
            (yyval.node)->kind = KIND_INT;
            }
#line 1858 "obj/grammar.c"
    break;

  case 55: /* F: CHARACTER  */
#line 476 "src/grammar.y"
                 {
        (yyval.node) = newNode(NODE_CHAR,NULL,NULL);
        (yyval.node)->charv = (yyvsp[0].charv);
        (yyval.node)->kind = KIND_CHAR;
    }
#line 1868 "obj/grammar.c"
    break;

  case 56: /* F: IDENT '(' Arguments ')'  */
#line 481 "src/grammar.y"
                               {
        (yyval.node) = newNode(NODE_FUNCTION_CALL,NULL,NULL);
        Node * second = newNode(NODE_IDENT,NULL,NULL);
        second->identv =strdup((yyvsp[-3].identv));
        second->kind = KIND_IDENT;
        addChild((yyval.node),second);
        addSibling(second,(yyvsp[-1].node));

         }
#line 1882 "obj/grammar.c"
    break;

  case 57: /* F: IDENT  */
#line 490 "src/grammar.y"
             {(yyval.node) = newNode(NODE_IDENT,NULL,NULL);
                (yyval.node)->kind = KIND_IDENT;
                (yyval.node)->identv = strdup((yyvsp[0].identv));
}
#line 1891 "obj/grammar.c"
    break;

  case 58: /* Arguments: ListExp  */
#line 498 "src/grammar.y"
               {(yyval.node) = (yyvsp[0].node);}
#line 1897 "obj/grammar.c"
    break;

  case 59: /* Arguments: %empty  */
#line 499 "src/grammar.y"
      {(yyval.node) = NULL;}
#line 1903 "obj/grammar.c"
    break;

  case 60: /* ListExp: ListExp ',' Exp  */
#line 502 "src/grammar.y"
                       {
                 (yyval.node) = (yyvsp[-2].node);
                 addSibling((yyval.node),(yyvsp[0].node));
                        }
#line 1912 "obj/grammar.c"
    break;

  case 61: /* ListExp: Exp  */
#line 506 "src/grammar.y"
           {(yyval.node) = (yyvsp[0].node);}
#line 1918 "obj/grammar.c"
    break;


#line 1922 "obj/grammar.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 508 "src/grammar.y"

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
