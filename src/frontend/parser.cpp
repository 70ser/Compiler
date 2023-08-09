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
#line 1 "parser.y"

    #include "../structure/ast.h"

    #include <string>
	#include <cstdio>
	#include <iostream>
	#include <cstdlib>
    #include <vector>

    AST::CompRoot *root;
    extern char *yytext;
    extern int yylex();
	extern int yyget_lineno();
	void yyerror(const char *s) { std::cerr << s << std::endl; exit(yytext[0]);}

#line 87 "parser.cpp"

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

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_RETURN = 3,                     /* RETURN  */
  YYSYMBOL_IF = 4,                         /* IF  */
  YYSYMBOL_ELSE = 5,                       /* ELSE  */
  YYSYMBOL_WHILE = 6,                      /* WHILE  */
  YYSYMBOL_BREAK = 7,                      /* BREAK  */
  YYSYMBOL_CONTINUE = 8,                   /* CONTINUE  */
  YYSYMBOL_VOIDT = 9,                      /* VOIDT  */
  YYSYMBOL_INTT = 10,                      /* INTT  */
  YYSYMBOL_FLOATT = 11,                    /* FLOATT  */
  YYSYMBOL_CONST = 12,                     /* CONST  */
  YYSYMBOL_INTNUMBER = 13,                 /* INTNUMBER  */
  YYSYMBOL_GETINT = 14,                    /* GETINT  */
  YYSYMBOL_GETCH = 15,                     /* GETCH  */
  YYSYMBOL_GETFLOAT = 16,                  /* GETFLOAT  */
  YYSYMBOL_GETARRAY = 17,                  /* GETARRAY  */
  YYSYMBOL_GETFARRAY = 18,                 /* GETFARRAY  */
  YYSYMBOL_PUTINT = 19,                    /* PUTINT  */
  YYSYMBOL_PUTCH = 20,                     /* PUTCH  */
  YYSYMBOL_PUTARRAY = 21,                  /* PUTARRAY  */
  YYSYMBOL_PUTFLOAT = 22,                  /* PUTFLOAT  */
  YYSYMBOL_PUTFARRAY = 23,                 /* PUTFARRAY  */
  YYSYMBOL_PUTF = 24,                      /* PUTF  */
  YYSYMBOL_STARTTIME = 25,                 /* STARTTIME  */
  YYSYMBOL_STOPTIME = 26,                  /* STOPTIME  */
  YYSYMBOL_FLOATNUMBER = 27,               /* FLOATNUMBER  */
  YYSYMBOL_ADDOP = 28,                     /* ADDOP  */
  YYSYMBOL_SUBOP = 29,                     /* SUBOP  */
  YYSYMBOL_NOTOP = 30,                     /* NOTOP  */
  YYSYMBOL_LANDOP = 31,                    /* LANDOP  */
  YYSYMBOL_LOROP = 32,                     /* LOROP  */
  YYSYMBOL_MULOP = 33,                     /* MULOP  */
  YYSYMBOL_EQUALOP = 34,                   /* EQUALOP  */
  YYSYMBOL_RELOP = 35,                     /* RELOP  */
  YYSYMBOL_ID = 36,                        /* ID  */
  YYSYMBOL_prec1 = 37,                     /* prec1  */
  YYSYMBOL_prec2 = 38,                     /* prec2  */
  YYSYMBOL_39_ = 39,                       /* ';'  */
  YYSYMBOL_40_ = 40,                       /* ','  */
  YYSYMBOL_41_ = 41,                       /* '='  */
  YYSYMBOL_42_ = 42,                       /* '['  */
  YYSYMBOL_43_ = 43,                       /* ']'  */
  YYSYMBOL_44_ = 44,                       /* '{'  */
  YYSYMBOL_45_ = 45,                       /* '}'  */
  YYSYMBOL_46_ = 46,                       /* '('  */
  YYSYMBOL_47_ = 47,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_COMPROOT = 49,                  /* COMPROOT  */
  YYSYMBOL_COMPUNIT = 50,                  /* COMPUNIT  */
  YYSYMBOL_DECL = 51,                      /* DECL  */
  YYSYMBOL_CONSTDECL = 52,                 /* CONSTDECL  */
  YYSYMBOL_CONSTDEFLIST = 53,              /* CONSTDEFLIST  */
  YYSYMBOL_BTYPE = 54,                     /* BTYPE  */
  YYSYMBOL_CONSTDEF = 55,                  /* CONSTDEF  */
  YYSYMBOL_VARDECL = 56,                   /* VARDECL  */
  YYSYMBOL_VARDEFLIST = 57,                /* VARDEFLIST  */
  YYSYMBOL_VARDEF = 58,                    /* VARDEF  */
  YYSYMBOL_ARRAYINDEXLIST = 59,            /* ARRAYINDEXLIST  */
  YYSYMBOL_INITVAL = 60,                   /* INITVAL  */
  YYSYMBOL_ARRAYINITIALVALS = 61,          /* ARRAYINITIALVALS  */
  YYSYMBOL_INITVALLIST = 62,               /* INITVALLIST  */
  YYSYMBOL_FUNCDEF = 63,                   /* FUNCDEF  */
  YYSYMBOL_PARAMETERS = 64,                /* PARAMETERS  */
  YYSYMBOL_PARAMETER = 65,                 /* PARAMETER  */
  YYSYMBOL_BLOCK = 66,                     /* BLOCK  */
  YYSYMBOL_BLOCKITEMLIST = 67,             /* BLOCKITEMLIST  */
  YYSYMBOL_BLOCKITEM = 68,                 /* BLOCKITEM  */
  YYSYMBOL_STMT = 69,                      /* STMT  */
  YYSYMBOL_ASSIGNSTMT = 70,                /* ASSIGNSTMT  */
  YYSYMBOL_EXPSTMT = 71,                   /* EXPSTMT  */
  YYSYMBOL_IFSTMT = 72,                    /* IFSTMT  */
  YYSYMBOL_WHILESTMT = 73,                 /* WHILESTMT  */
  YYSYMBOL_BREAKSTMT = 74,                 /* BREAKSTMT  */
  YYSYMBOL_CONTINUESTMT = 75,              /* CONTINUESTMT  */
  YYSYMBOL_RETURNSTMT = 76,                /* RETURNSTMT  */
  YYSYMBOL_PUTINTSTMT = 77,                /* PUTINTSTMT  */
  YYSYMBOL_PUTCHSTMT = 78,                 /* PUTCHSTMT  */
  YYSYMBOL_PUTFLOATSTMT = 79,              /* PUTFLOATSTMT  */
  YYSYMBOL_PUTARRAYSTMT = 80,              /* PUTARRAYSTMT  */
  YYSYMBOL_PUTFARRAYSTMT = 81,             /* PUTFARRAYSTMT  */
  YYSYMBOL_PUTFSTMT = 82,                  /* PUTFSTMT  */
  YYSYMBOL_STARTTIMESTMT = 83,             /* STARTTIMESTMT  */
  YYSYMBOL_STOPTIMESTMT = 84,              /* STOPTIMESTMT  */
  YYSYMBOL_EXP = 85,                       /* EXP  */
  YYSYMBOL_IDEXP = 86,                     /* IDEXP  */
  YYSYMBOL_LVAL = 87,                      /* LVAL  */
  YYSYMBOL_PRIMARYEXP = 88,                /* PRIMARYEXP  */
  YYSYMBOL_NUMBER = 89,                    /* NUMBER  */
  YYSYMBOL_UNARYEXP = 90,                  /* UNARYEXP  */
  YYSYMBOL_CALLEXP = 91,                   /* CALLEXP  */
  YYSYMBOL_UNARYOP = 92,                   /* UNARYOP  */
  YYSYMBOL_EXPLIST = 93,                   /* EXPLIST  */
  YYSYMBOL_MULEXP = 94,                    /* MULEXP  */
  YYSYMBOL_ADDEXP = 95,                    /* ADDEXP  */
  YYSYMBOL_RELEXP = 96,                    /* RELEXP  */
  YYSYMBOL_EQEXP = 97,                     /* EQEXP  */
  YYSYMBOL_LANDEXP = 98,                   /* LANDEXP  */
  YYSYMBOL_LOREXP = 99,                    /* LOREXP  */
  YYSYMBOL_GETINTEXP = 100,                /* GETINTEXP  */
  YYSYMBOL_GETCHEXP = 101,                 /* GETCHEXP  */
  YYSYMBOL_GETFLOATEXP = 102,              /* GETFLOATEXP  */
  YYSYMBOL_GETARRAYEXP = 103,              /* GETARRAYEXP  */
  YYSYMBOL_GETFARRAYEXP = 104              /* GETFARRAYEXP  */
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
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   348

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  210

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   293


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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      46,    47,     2,     2,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    39,
       2,    41,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    42,     2,    43,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,     2,    45,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   125,   125,   129,   135,   137,   143,   145,   149,   153,
     156,   161,   163,   165,   169,   173,   177,   180,   185,   187,
     192,   194,   199,   201,   206,   210,   212,   215,   224,   228,
     230,   233,   238,   240,   250,   254,   256,   261,   263,   267,
     269,   271,   273,   275,   277,   279,   281,   283,   285,   287,
     289,   291,   293,   295,   297,   299,   303,   307,   311,   313,
     317,   321,   325,   329,   331,   335,   339,   343,   347,   351,
     355,   359,   363,   367,   369,   371,   373,   375,   377,   379,
     381,   383,   385,   387,   389,   391,   393,   397,   401,   405,
     407,   409,   413,   415,   419,   423,   427,   429,   431,   435,
     437,   440,   445,   449,   451,   455,   459,   463,   467,   471,
     475,   479,   483,   487
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
  "\"end of file\"", "error", "\"invalid token\"", "RETURN", "IF", "ELSE",
  "WHILE", "BREAK", "CONTINUE", "VOIDT", "INTT", "FLOATT", "CONST",
  "INTNUMBER", "GETINT", "GETCH", "GETFLOAT", "GETARRAY", "GETFARRAY",
  "PUTINT", "PUTCH", "PUTARRAY", "PUTFLOAT", "PUTFARRAY", "PUTF",
  "STARTTIME", "STOPTIME", "FLOATNUMBER", "ADDOP", "SUBOP", "NOTOP",
  "LANDOP", "LOROP", "MULOP", "EQUALOP", "RELOP", "ID", "prec1", "prec2",
  "';'", "','", "'='", "'['", "']'", "'{'", "'}'", "'('", "')'", "$accept",
  "COMPROOT", "COMPUNIT", "DECL", "CONSTDECL", "CONSTDEFLIST", "BTYPE",
  "CONSTDEF", "VARDECL", "VARDEFLIST", "VARDEF", "ARRAYINDEXLIST",
  "INITVAL", "ARRAYINITIALVALS", "INITVALLIST", "FUNCDEF", "PARAMETERS",
  "PARAMETER", "BLOCK", "BLOCKITEMLIST", "BLOCKITEM", "STMT", "ASSIGNSTMT",
  "EXPSTMT", "IFSTMT", "WHILESTMT", "BREAKSTMT", "CONTINUESTMT",
  "RETURNSTMT", "PUTINTSTMT", "PUTCHSTMT", "PUTFLOATSTMT", "PUTARRAYSTMT",
  "PUTFARRAYSTMT", "PUTFSTMT", "STARTTIMESTMT", "STOPTIMESTMT", "EXP",
  "IDEXP", "LVAL", "PRIMARYEXP", "NUMBER", "UNARYEXP", "CALLEXP",
  "UNARYOP", "EXPLIST", "MULEXP", "ADDEXP", "RELEXP", "EQEXP", "LANDEXP",
  "LOREXP", "GETINTEXP", "GETCHEXP", "GETFLOATEXP", "GETARRAYEXP",
  "GETFARRAYEXP", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-188)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      24,  -188,  -188,  -188,   100,    94,  -188,  -188,  -188,   -24,
    -188,  -188,   -24,  -188,  -188,  -188,   -14,  -188,   -41,    22,
    -188,  -188,  -188,   -24,   100,    38,  -188,   -24,    71,  -188,
    -188,   -24,   -31,  -188,    72,   234,  -188,    72,   -27,   100,
     -15,  -188,    -5,     4,     6,    45,    76,  -188,  -188,  -188,
    -188,    72,   234,  -188,  -188,   309,    77,  -188,  -188,  -188,
    -188,  -188,   234,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,   118,  -188,   -23,  -188,  -188,  -188,
     -16,     0,    78,   234,   234,  -188,   -34,    35,   234,   234,
     234,   234,   234,   234,   234,   234,    84,  -188,  -188,  -188,
     166,  -188,  -188,  -188,   109,   225,    72,  -188,  -188,   102,
     102,   240,    86,  -188,   129,    64,   309,   -26,    84,   234,
      93,    99,  -188,  -188,   108,   120,   121,   122,   151,   152,
    -188,  -188,  -188,  -188,  -188,   -24,  -188,  -188,  -188,   116,
     160,  -188,  -188,   161,   162,   170,   192,   193,   194,   195,
     196,   198,   199,   201,   309,   202,  -188,  -188,  -188,   234,
    -188,   309,   234,   234,   234,   234,   234,   234,   234,   234,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,   234,   309,   250,   258,   267,   275,    11,
     284,    43,   -17,   309,   200,   200,  -188,  -188,   234,  -188,
     234,  -188,   236,  -188,   292,   301,   200,  -188,  -188,  -188
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    13,    11,    12,     0,     0,     3,     4,     6,     0,
       7,     5,     0,     1,     2,    87,     0,    16,    20,     0,
       9,    20,    15,     0,    29,    18,     8,     0,     0,    17,
      20,     0,     0,    30,     0,     0,    10,     0,    32,     0,
       0,    92,     0,     0,     0,     0,     0,    93,    96,    97,
      98,    25,     0,    19,    23,    22,    20,    90,    73,    91,
      74,    81,     0,    75,    76,    77,    78,    79,    80,    82,
      83,    84,    85,    86,     0,    14,     0,    31,    35,    28,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,    88,    94,    21,    20,
       0,   109,   110,   111,     0,     0,     0,    24,    89,   103,
     104,   107,   108,   102,   106,   105,   100,     0,    33,    63,
       0,     0,    61,    62,     0,     0,     0,     0,     0,     0,
      71,    72,    40,    34,    37,     0,    42,    36,    38,     0,
       0,    43,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    90,   112,   113,    27,     0,
      95,    64,     0,     0,     0,     0,     0,     0,     0,    99,
      39,    41,    45,    46,    47,    48,    49,    51,    50,    52,
      53,    54,    55,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,    65,    66,     0,    67,
       0,    70,    58,    60,     0,     0,     0,    68,    69,    59
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -188,  -188,   237,   145,  -188,  -188,    -2,   228,  -188,  -188,
     242,   -18,   -33,  -188,  -188,  -188,  -188,   227,   231,  -188,
    -188,  -187,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,   -35,     1,   -99,
    -188,  -188,  -188,  -188,  -188,    98,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,     7,     8,    19,     9,    20,    10,    16,
      17,    25,    53,    54,    86,    11,    32,    33,   136,   100,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    55,    56,    57,
      58,    59,    60,    61,    62,   117,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      74,   155,    12,    28,    75,    24,   106,   202,   203,    39,
      18,   107,    15,    21,   159,    76,    40,    87,    85,   209,
      99,   160,    31,   159,    30,    22,    23,    97,    21,    78,
     201,   101,    38,     1,     2,     3,     4,    31,    96,    88,
      89,    80,    90,    91,    92,    93,    94,   102,   104,   105,
      81,   198,    82,   109,   110,   111,   112,   113,   114,   115,
     116,    26,    27,    88,    89,   154,    90,    91,    92,    93,
      94,    88,    89,   158,    90,    91,    92,    93,    94,    34,
      35,   118,   108,   200,   161,    41,    42,    43,    44,    45,
      46,    83,    88,    89,    13,   155,   155,    92,   135,    47,
      48,    49,    50,     1,     2,     3,     4,   155,    15,     1,
       2,     3,    37,    35,    88,    89,    51,    90,    52,    92,
      93,    94,    84,    95,   184,   103,    35,   185,   186,   187,
     188,   189,   190,   191,   116,    92,    30,    88,    89,   162,
      90,    91,    92,    93,    94,   163,    88,    89,   193,    90,
      91,    92,    93,    94,   164,   170,   156,    88,    89,   154,
     154,    98,    92,   204,    94,   205,   165,   166,   167,   119,
     120,   154,   121,   122,   123,     1,     2,     3,     4,    41,
      42,    43,    44,    45,    46,   124,   125,   126,   127,   128,
     129,   130,   131,    47,    48,    49,    50,   168,   169,   171,
     172,   173,    15,   119,   120,   132,   121,   122,   123,   174,
      78,   133,    52,    41,    42,    43,    44,    45,    46,   124,
     125,   126,   127,   128,   129,   130,   131,    47,    48,    49,
      50,   175,   176,   177,   178,   179,    15,   180,   181,   132,
     182,   206,    14,   183,    78,   134,    52,    41,    42,    43,
      44,    45,    46,    88,    89,    36,    90,    91,    92,    93,
      94,    47,    48,    49,    50,    29,    77,   192,    88,    89,
      15,    79,   157,    92,    93,    94,     0,     0,    88,    89,
      52,    90,    91,    92,    93,    94,    88,    89,     0,    90,
      91,    92,    93,    94,     0,    88,    89,   194,    90,    91,
      92,    93,    94,    88,    89,   195,    90,    91,    92,    93,
      94,     0,    88,    89,   196,    90,    91,    92,    93,    94,
      88,    89,   197,    90,    91,    92,    93,    94,     0,    88,
      89,   199,    90,    91,    92,    93,    94,    88,    89,   207,
      90,    91,    92,    93,    94,     0,     0,     0,   208
};

static const yytype_int16 yycheck[] =
{
      35,   100,     4,    21,    37,    46,    40,   194,   195,    40,
       9,    45,    36,    12,    40,    42,    47,    52,    51,   206,
      43,    47,    24,    40,    23,    39,    40,    62,    27,    44,
      47,    47,    31,     9,    10,    11,    12,    39,    56,    28,
      29,    46,    31,    32,    33,    34,    35,    47,    83,    84,
      46,    40,    46,    88,    89,    90,    91,    92,    93,    94,
      95,    39,    40,    28,    29,   100,    31,    32,    33,    34,
      35,    28,    29,   106,    31,    32,    33,    34,    35,    41,
      42,    99,    47,    40,   119,    13,    14,    15,    16,    17,
      18,    46,    28,    29,     0,   194,   195,    33,   100,    27,
      28,    29,    30,     9,    10,    11,    12,   206,    36,     9,
      10,    11,    41,    42,    28,    29,    44,    31,    46,    33,
      34,    35,    46,    46,   159,    47,    42,   162,   163,   164,
     165,   166,   167,   168,   169,    33,   135,    28,    29,    46,
      31,    32,    33,    34,    35,    46,    28,    29,   183,    31,
      32,    33,    34,    35,    46,    39,    47,    28,    29,   194,
     195,    43,    33,   198,    35,   200,    46,    46,    46,     3,
       4,   206,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    46,    46,    39,
      39,    39,    36,     3,     4,    39,     6,     7,     8,    39,
      44,    45,    46,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    39,    39,    39,    39,    39,    36,    39,    39,    39,
      39,     5,     5,    41,    44,   100,    46,    13,    14,    15,
      16,    17,    18,    28,    29,    27,    31,    32,    33,    34,
      35,    27,    28,    29,    30,    23,    39,   169,    28,    29,
      36,    40,    47,    33,    34,    35,    -1,    -1,    28,    29,
      46,    31,    32,    33,    34,    35,    28,    29,    -1,    31,
      32,    33,    34,    35,    -1,    28,    29,    47,    31,    32,
      33,    34,    35,    28,    29,    47,    31,    32,    33,    34,
      35,    -1,    28,    29,    47,    31,    32,    33,    34,    35,
      28,    29,    47,    31,    32,    33,    34,    35,    -1,    28,
      29,    47,    31,    32,    33,    34,    35,    28,    29,    47,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    47
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    10,    11,    12,    49,    50,    51,    52,    54,
      56,    63,    54,     0,    50,    36,    57,    58,    86,    53,
      55,    86,    39,    40,    46,    59,    39,    40,    59,    58,
      86,    54,    64,    65,    41,    42,    55,    41,    86,    40,
      47,    13,    14,    15,    16,    17,    18,    27,    28,    29,
      30,    44,    46,    60,    61,    85,    86,    87,    88,    89,
      90,    91,    92,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,    85,    60,    42,    65,    44,    66,
      46,    46,    46,    46,    46,    60,    62,    85,    28,    29,
      31,    32,    33,    34,    35,    46,    59,    85,    43,    43,
      67,    47,    47,    47,    85,    85,    40,    45,    47,    85,
      85,    85,    85,    85,    85,    85,    85,    93,    59,     3,
       4,     6,     7,     8,    19,    20,    21,    22,    23,    24,
      25,    26,    39,    45,    51,    54,    66,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    87,    47,    47,    60,    40,
      47,    85,    46,    46,    46,    46,    46,    46,    46,    46,
      39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    41,    85,    85,    85,    85,    85,    85,
      85,    85,    93,    85,    47,    47,    47,    47,    40,    47,
      40,    47,    69,    69,    85,    85,     5,    47,    47,    69
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    49,    50,    50,    51,    51,    52,    53,
      53,    54,    54,    54,    55,    56,    57,    57,    58,    58,
      59,    59,    60,    60,    61,    62,    62,    62,    63,    64,
      64,    64,    65,    65,    66,    67,    67,    68,    68,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    70,    71,    72,    72,
      73,    74,    75,    76,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    86,    87,    88,
      88,    88,    89,    89,    90,    91,    92,    92,    92,    93,
      93,    93,    94,    95,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     4,     1,
       3,     1,     1,     1,     4,     3,     1,     3,     2,     4,
       0,     4,     1,     1,     3,     0,     1,     3,     6,     0,
       1,     3,     2,     5,     3,     0,     2,     1,     1,     2,
       1,     2,     1,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     1,     5,     7,
       5,     1,     1,     1,     2,     4,     4,     4,     6,     6,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     3,
       1,     1,     1,     1,     2,     4,     1,     1,     1,     0,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     4
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
  case 2: /* COMPROOT: COMPROOT COMPUNIT  */
#line 125 "parser.y"
                            {
    (yyval.root) = (yyvsp[-1].root);
    (yyval.root)->comp_list.push_back((yyvsp[0].compunit));
    root = (yyval.root);
}
#line 1359 "parser.cpp"
    break;

  case 3: /* COMPROOT: COMPUNIT  */
#line 129 "parser.y"
             {
    (yyval.root) = new AST::CompRoot(yyget_lineno());
    (yyval.root)->comp_list.push_back((yyvsp[0].compunit));
    root = (yyval.root);
}
#line 1369 "parser.cpp"
    break;

  case 4: /* COMPUNIT: DECL  */
#line 135 "parser.y"
                {
	(yyval.compunit) = static_cast<AST::CompUnit *>((yyvsp[0].decl));
}
#line 1377 "parser.cpp"
    break;

  case 5: /* COMPUNIT: FUNCDEF  */
#line 137 "parser.y"
            {
	(yyval.compunit) = static_cast<AST::CompUnit *>((yyvsp[0].funcdef));
}
#line 1385 "parser.cpp"
    break;

  case 6: /* DECL: CONSTDECL  */
#line 143 "parser.y"
                {
	(yyval.decl) = static_cast<AST::Decl *>((yyvsp[0].constdecl));
}
#line 1393 "parser.cpp"
    break;

  case 7: /* DECL: VARDECL  */
#line 145 "parser.y"
            {
	(yyval.decl) = static_cast<AST::Decl *>((yyvsp[0].vardecl));
}
#line 1401 "parser.cpp"
    break;

  case 8: /* CONSTDECL: CONST BTYPE CONSTDEFLIST ';'  */
#line 149 "parser.y"
                                        {
	(yyval.constdecl) = new AST::ConstDecl((yyvsp[-2].btype), (yyvsp[-1].constdeflist), yyget_lineno());
}
#line 1409 "parser.cpp"
    break;

  case 9: /* CONSTDEFLIST: CONSTDEF  */
#line 153 "parser.y"
                       {
	(yyval.constdeflist) = new std::vector<AST::ConstDef*>();
	(yyval.constdeflist)->push_back((yyvsp[0].constdef));
}
#line 1418 "parser.cpp"
    break;

  case 10: /* CONSTDEFLIST: CONSTDEFLIST ',' CONSTDEF  */
#line 156 "parser.y"
                              {
	(yyval.constdeflist) = (yyvsp[-2].constdeflist);
	(yyvsp[-2].constdeflist)->push_back((yyvsp[0].constdef));
}
#line 1427 "parser.cpp"
    break;

  case 11: /* BTYPE: INTT  */
#line 161 "parser.y"
            {
	(yyval.btype) = AST::base_t::INT;
}
#line 1435 "parser.cpp"
    break;

  case 12: /* BTYPE: FLOATT  */
#line 163 "parser.y"
           {
	(yyval.btype) = AST::base_t::FLOAT;
}
#line 1443 "parser.cpp"
    break;

  case 13: /* BTYPE: VOIDT  */
#line 165 "parser.y"
          {
	(yyval.btype) = AST::base_t::VOID;
}
#line 1451 "parser.cpp"
    break;

  case 14: /* CONSTDEF: IDEXP ARRAYINDEXLIST '=' INITVAL  */
#line 169 "parser.y"
                                           {
	(yyval.constdef) = new AST::ConstDef((yyvsp[-3].idexp), (yyvsp[-2].arrayindexlist), (yyvsp[0].initval), yyget_lineno());
}
#line 1459 "parser.cpp"
    break;

  case 15: /* VARDECL: BTYPE VARDEFLIST ';'  */
#line 173 "parser.y"
                              {
	(yyval.vardecl) = new AST::VarDecl((yyvsp[-2].btype), (yyvsp[-1].vardeflist), yyget_lineno());
}
#line 1467 "parser.cpp"
    break;

  case 16: /* VARDEFLIST: VARDEF  */
#line 177 "parser.y"
                   {
	(yyval.vardeflist) = new std::vector<AST::VarDef*>();
	(yyval.vardeflist)->push_back((yyvsp[0].vardef));
}
#line 1476 "parser.cpp"
    break;

  case 17: /* VARDEFLIST: VARDEFLIST ',' VARDEF  */
#line 180 "parser.y"
                          {
	(yyval.vardeflist) = (yyvsp[-2].vardeflist);
	(yyvsp[-2].vardeflist)->push_back((yyvsp[0].vardef));
}
#line 1485 "parser.cpp"
    break;

  case 18: /* VARDEF: IDEXP ARRAYINDEXLIST  */
#line 185 "parser.y"
                             {
	(yyval.vardef) = new AST::VarDef((yyvsp[-1].idexp), (yyvsp[0].arrayindexlist), nullptr, yyget_lineno());
}
#line 1493 "parser.cpp"
    break;

  case 19: /* VARDEF: IDEXP ARRAYINDEXLIST '=' INITVAL  */
#line 187 "parser.y"
                                     {
	(yyval.vardef) = new AST::VarDef((yyvsp[-3].idexp), (yyvsp[-2].arrayindexlist), (yyvsp[0].initval), yyget_lineno());
}
#line 1501 "parser.cpp"
    break;

  case 20: /* ARRAYINDEXLIST: %empty  */
#line 192 "parser.y"
                 {
	(yyval.arrayindexlist) = new std::vector<AST::Exp*>();
}
#line 1509 "parser.cpp"
    break;

  case 21: /* ARRAYINDEXLIST: ARRAYINDEXLIST '[' EXP ']'  */
#line 194 "parser.y"
                               {
	(yyval.arrayindexlist) = (yyvsp[-3].arrayindexlist);
	(yyval.arrayindexlist)->push_back((yyvsp[-1].exp));
}
#line 1518 "parser.cpp"
    break;

  case 22: /* INITVAL: EXP  */
#line 199 "parser.y"
             {
	(yyval.initval) = (yyvsp[0].exp);
}
#line 1526 "parser.cpp"
    break;

  case 23: /* INITVAL: ARRAYINITIALVALS  */
#line 201 "parser.y"
                     {
	(yyval.initval) = (yyvsp[0].arrayinitialvals);
}
#line 1534 "parser.cpp"
    break;

  case 24: /* ARRAYINITIALVALS: '{' INITVALLIST '}'  */
#line 206 "parser.y"
                                       {
	(yyval.arrayinitialvals) = new AST::ArrayInitVals(yyget_lineno(), (yyvsp[-1].initvallist));
}
#line 1542 "parser.cpp"
    break;

  case 25: /* INITVALLIST: %empty  */
#line 210 "parser.y"
             {
	(yyval.initvallist) = new std::vector<AST::InitVal *>();
}
#line 1550 "parser.cpp"
    break;

  case 26: /* INITVALLIST: INITVAL  */
#line 212 "parser.y"
            {
	(yyval.initvallist) = new std::vector<AST::InitVal *>();
    (yyval.initvallist)->push_back((yyvsp[0].initval));
}
#line 1559 "parser.cpp"
    break;

  case 27: /* INITVALLIST: INITVALLIST ',' INITVAL  */
#line 215 "parser.y"
                            {
	(yyval.initvallist) = (yyvsp[-2].initvallist);
	(yyval.initvallist)->push_back((yyvsp[0].initval));
}
#line 1568 "parser.cpp"
    break;

  case 28: /* FUNCDEF: BTYPE IDEXP '(' PARAMETERS ')' BLOCK  */
#line 224 "parser.y"
                                              {
	(yyval.funcdef) = new AST::FuncDef((yyvsp[-5].btype), (yyvsp[-4].idexp), (yyvsp[-2].parameters), (yyvsp[0].block), yyget_lineno());
}
#line 1576 "parser.cpp"
    break;

  case 29: /* PARAMETERS: %empty  */
#line 228 "parser.y"
             {
	(yyval.parameters) = new std::vector<AST::Parameter*>();
}
#line 1584 "parser.cpp"
    break;

  case 30: /* PARAMETERS: PARAMETER  */
#line 230 "parser.y"
              {
    (yyval.parameters) = new std::vector<AST::Parameter*>();
    (yyval.parameters)->push_back((yyvsp[0].parameter));
}
#line 1593 "parser.cpp"
    break;

  case 31: /* PARAMETERS: PARAMETERS ',' PARAMETER  */
#line 233 "parser.y"
                             {
	(yyval.parameters) = (yyvsp[-2].parameters);
	(yyval.parameters)->push_back((yyvsp[0].parameter));
}
#line 1602 "parser.cpp"
    break;

  case 32: /* PARAMETER: BTYPE IDEXP  */
#line 238 "parser.y"
                       {
	(yyval.parameter) = new AST::Parameter((yyvsp[-1].btype), (yyvsp[0].idexp), new std::vector<AST::Exp *>(), yyget_lineno());
}
#line 1610 "parser.cpp"
    break;

  case 33: /* PARAMETER: BTYPE IDEXP '[' ']' ARRAYINDEXLIST  */
#line 240 "parser.y"
                                       {
    // int a[][3]: define 1's demension as 1
	std::vector<AST::Exp *> *x = new std::vector<AST::Exp *>();
	x->push_back(new AST::IntNumber(1, yyget_lineno()));
	for (const auto &y : *(yyvsp[0].arrayindexlist)) {
		x->push_back(y);
	}
	(yyval.parameter) = new AST::Parameter((yyvsp[-4].btype), (yyvsp[-3].idexp), x, yyget_lineno());
}
#line 1624 "parser.cpp"
    break;

  case 34: /* BLOCK: '{' BLOCKITEMLIST '}'  */
#line 250 "parser.y"
                             {
	(yyval.block) = new AST::Block((yyvsp[-1].blockitemlist), yyget_lineno());
}
#line 1632 "parser.cpp"
    break;

  case 35: /* BLOCKITEMLIST: %empty  */
#line 254 "parser.y"
                {
	(yyval.blockitemlist) = new std::vector<AST::BlockItem *>();
}
#line 1640 "parser.cpp"
    break;

  case 36: /* BLOCKITEMLIST: BLOCKITEMLIST BLOCKITEM  */
#line 256 "parser.y"
                            {
	(yyval.blockitemlist) = (yyvsp[-1].blockitemlist);
	(yyval.blockitemlist)->push_back((yyvsp[0].blockitem));
}
#line 1649 "parser.cpp"
    break;

  case 37: /* BLOCKITEM: DECL  */
#line 261 "parser.y"
                {
	(yyval.blockitem) = (yyvsp[0].decl);
}
#line 1657 "parser.cpp"
    break;

  case 38: /* BLOCKITEM: STMT  */
#line 263 "parser.y"
         {
	(yyval.blockitem) = (yyvsp[0].stmt);
}
#line 1665 "parser.cpp"
    break;

  case 39: /* STMT: ASSIGNSTMT ';'  */
#line 267 "parser.y"
                     {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1673 "parser.cpp"
    break;

  case 40: /* STMT: ';'  */
#line 269 "parser.y"
        {
	(yyval.stmt) = new AST::ExpStmt(new AST::IntNumber(0, yyget_lineno()), yyget_lineno());
}
#line 1681 "parser.cpp"
    break;

  case 41: /* STMT: EXPSTMT ';'  */
#line 271 "parser.y"
                {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1689 "parser.cpp"
    break;

  case 42: /* STMT: BLOCK  */
#line 273 "parser.y"
          {
	(yyval.stmt) = (yyvsp[0].block);
}
#line 1697 "parser.cpp"
    break;

  case 43: /* STMT: IFSTMT  */
#line 275 "parser.y"
           {
	(yyval.stmt) = (yyvsp[0].stmt);
}
#line 1705 "parser.cpp"
    break;

  case 44: /* STMT: WHILESTMT  */
#line 277 "parser.y"
              {
	(yyval.stmt) = (yyvsp[0].stmt);
}
#line 1713 "parser.cpp"
    break;

  case 45: /* STMT: BREAKSTMT ';'  */
#line 279 "parser.y"
                  {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1721 "parser.cpp"
    break;

  case 46: /* STMT: CONTINUESTMT ';'  */
#line 281 "parser.y"
                     {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1729 "parser.cpp"
    break;

  case 47: /* STMT: RETURNSTMT ';'  */
#line 283 "parser.y"
                   {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1737 "parser.cpp"
    break;

  case 48: /* STMT: PUTINTSTMT ';'  */
#line 285 "parser.y"
                   {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1745 "parser.cpp"
    break;

  case 49: /* STMT: PUTCHSTMT ';'  */
#line 287 "parser.y"
                  {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1753 "parser.cpp"
    break;

  case 50: /* STMT: PUTARRAYSTMT ';'  */
#line 289 "parser.y"
                     {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1761 "parser.cpp"
    break;

  case 51: /* STMT: PUTFLOATSTMT ';'  */
#line 291 "parser.y"
                     {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1769 "parser.cpp"
    break;

  case 52: /* STMT: PUTFARRAYSTMT ';'  */
#line 293 "parser.y"
                      {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1777 "parser.cpp"
    break;

  case 53: /* STMT: PUTFSTMT ';'  */
#line 295 "parser.y"
                 {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1785 "parser.cpp"
    break;

  case 54: /* STMT: STARTTIMESTMT ';'  */
#line 297 "parser.y"
                      {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1793 "parser.cpp"
    break;

  case 55: /* STMT: STOPTIMESTMT ';'  */
#line 299 "parser.y"
                     {
	(yyval.stmt) = (yyvsp[-1].stmt);
}
#line 1801 "parser.cpp"
    break;

  case 56: /* ASSIGNSTMT: LVAL '=' EXP  */
#line 303 "parser.y"
                         {
	(yyval.stmt) = new AST::AssignStmt((yyvsp[-2].lval), (yyvsp[0].exp), yyget_lineno());
}
#line 1809 "parser.cpp"
    break;

  case 57: /* EXPSTMT: EXP  */
#line 307 "parser.y"
             {
	(yyval.stmt) = new AST::ExpStmt((yyvsp[0].exp), yyget_lineno());
}
#line 1817 "parser.cpp"
    break;

  case 58: /* IFSTMT: IF '(' EXP ')' STMT  */
#line 311 "parser.y"
                                        {
	(yyval.stmt) = new AST::IfStmt((yyvsp[-2].exp), (yyvsp[0].stmt), nullptr, yyget_lineno());
}
#line 1825 "parser.cpp"
    break;

  case 59: /* IFSTMT: IF '(' EXP ')' STMT ELSE STMT  */
#line 313 "parser.y"
                                  {
	(yyval.stmt) = new AST::IfStmt((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), yyget_lineno());
}
#line 1833 "parser.cpp"
    break;

  case 60: /* WHILESTMT: WHILE '(' EXP ')' STMT  */
#line 317 "parser.y"
                                  {
	(yyval.stmt) = new AST::WhileStmt((yyvsp[-2].exp), (yyvsp[0].stmt), yyget_lineno());
}
#line 1841 "parser.cpp"
    break;

  case 61: /* BREAKSTMT: BREAK  */
#line 321 "parser.y"
                 {
	(yyval.stmt) = new AST::BreakStmt(yyget_lineno());
}
#line 1849 "parser.cpp"
    break;

  case 62: /* CONTINUESTMT: CONTINUE  */
#line 325 "parser.y"
                       {
	(yyval.stmt) = new AST::ContinueStmt(yyget_lineno());
}
#line 1857 "parser.cpp"
    break;

  case 63: /* RETURNSTMT: RETURN  */
#line 329 "parser.y"
                   {
	(yyval.stmt) = new AST::ReturnStmt(nullptr, yyget_lineno());
}
#line 1865 "parser.cpp"
    break;

  case 64: /* RETURNSTMT: RETURN EXP  */
#line 331 "parser.y"
               {
	(yyval.stmt) = new AST::ReturnStmt((yyvsp[0].exp), yyget_lineno());
}
#line 1873 "parser.cpp"
    break;

  case 65: /* PUTINTSTMT: PUTINT '(' EXP ')'  */
#line 335 "parser.y"
                               {
	(yyval.stmt) = new AST::PutIntStmt((yyvsp[-1].exp), yyget_lineno());
}
#line 1881 "parser.cpp"
    break;

  case 66: /* PUTCHSTMT: PUTCH '(' EXP ')'  */
#line 339 "parser.y"
                             {
	(yyval.stmt) = new AST::PutChStmt((yyvsp[-1].exp), yyget_lineno());
}
#line 1889 "parser.cpp"
    break;

  case 67: /* PUTFLOATSTMT: PUTFLOAT '(' EXP ')'  */
#line 343 "parser.y"
                                   {
	(yyval.stmt) = new AST::PutFloatStmt((yyvsp[-1].exp), yyget_lineno());
}
#line 1897 "parser.cpp"
    break;

  case 68: /* PUTARRAYSTMT: PUTARRAY '(' EXP ',' EXP ')'  */
#line 347 "parser.y"
                                           {
	(yyval.stmt) = new AST::PutArrayStmt((yyvsp[-3].exp), (yyvsp[-1].exp), yyget_lineno());
}
#line 1905 "parser.cpp"
    break;

  case 69: /* PUTFARRAYSTMT: PUTFARRAY '(' EXP ',' EXP ')'  */
#line 351 "parser.y"
                                             {
	(yyval.stmt) = new AST::PutFArrayStmt((yyvsp[-3].exp), (yyvsp[-1].exp), yyget_lineno());
}
#line 1913 "parser.cpp"
    break;

  case 70: /* PUTFSTMT: PUTF '(' EXPLIST ')'  */
#line 355 "parser.y"
                               {
	(yyval.stmt) = new AST::PutFStmt((yyvsp[-1].explist), yyget_lineno());
}
#line 1921 "parser.cpp"
    break;

  case 71: /* STARTTIMESTMT: STARTTIME  */
#line 359 "parser.y"
                         {
	(yyval.stmt) = new AST::StarttimeStmt(yyget_lineno());
}
#line 1929 "parser.cpp"
    break;

  case 72: /* STOPTIMESTMT: STOPTIME  */
#line 363 "parser.y"
                       {
	(yyval.stmt) = new AST::StoptimeStmt(yyget_lineno());
}
#line 1937 "parser.cpp"
    break;

  case 73: /* EXP: PRIMARYEXP  */
#line 367 "parser.y"
                {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 1945 "parser.cpp"
    break;

  case 74: /* EXP: UNARYEXP  */
#line 369 "parser.y"
             {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 1953 "parser.cpp"
    break;

  case 75: /* EXP: MULEXP  */
#line 371 "parser.y"
           {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 1961 "parser.cpp"
    break;

  case 76: /* EXP: ADDEXP  */
#line 373 "parser.y"
           {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 1969 "parser.cpp"
    break;

  case 77: /* EXP: RELEXP  */
#line 375 "parser.y"
           {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 1977 "parser.cpp"
    break;

  case 78: /* EXP: EQEXP  */
#line 377 "parser.y"
          {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 1985 "parser.cpp"
    break;

  case 79: /* EXP: LANDEXP  */
#line 379 "parser.y"
            {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 1993 "parser.cpp"
    break;

  case 80: /* EXP: LOREXP  */
#line 381 "parser.y"
           {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 2001 "parser.cpp"
    break;

  case 81: /* EXP: CALLEXP  */
#line 383 "parser.y"
            {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 2009 "parser.cpp"
    break;

  case 82: /* EXP: GETINTEXP  */
#line 385 "parser.y"
              {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 2017 "parser.cpp"
    break;

  case 83: /* EXP: GETCHEXP  */
#line 387 "parser.y"
             {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 2025 "parser.cpp"
    break;

  case 84: /* EXP: GETFLOATEXP  */
#line 389 "parser.y"
                {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 2033 "parser.cpp"
    break;

  case 85: /* EXP: GETARRAYEXP  */
#line 391 "parser.y"
                {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 2041 "parser.cpp"
    break;

  case 86: /* EXP: GETFARRAYEXP  */
#line 393 "parser.y"
                 {
	(yyval.exp) = (yyvsp[0].exp);
}
#line 2049 "parser.cpp"
    break;

  case 87: /* IDEXP: ID  */
#line 397 "parser.y"
           {
    (yyval.idexp) = new AST::IDExp((yyvsp[0].id), yyget_lineno());
}
#line 2057 "parser.cpp"
    break;

  case 88: /* LVAL: IDEXP ARRAYINDEXLIST  */
#line 401 "parser.y"
                           {
	(yyval.lval) = new AST::LVal((yyvsp[-1].idexp), (yyvsp[0].arrayindexlist), yyget_lineno());
}
#line 2065 "parser.cpp"
    break;

  case 89: /* PRIMARYEXP: '(' EXP ')'  */
#line 405 "parser.y"
                        {
	(yyval.exp) = (yyvsp[-1].exp);
}
#line 2073 "parser.cpp"
    break;

  case 90: /* PRIMARYEXP: LVAL  */
#line 407 "parser.y"
         {
	(yyval.exp) = (yyvsp[0].lval);
}
#line 2081 "parser.cpp"
    break;

  case 91: /* PRIMARYEXP: NUMBER  */
#line 409 "parser.y"
           {
	(yyval.exp) = (yyvsp[0].number);
}
#line 2089 "parser.cpp"
    break;

  case 92: /* NUMBER: INTNUMBER  */
#line 413 "parser.y"
                  {
	(yyval.number) = new AST::IntNumber((yyvsp[0].token), yyget_lineno());
}
#line 2097 "parser.cpp"
    break;

  case 93: /* NUMBER: FLOATNUMBER  */
#line 415 "parser.y"
                {
	(yyval.number) = new AST::FloatNumber((yyvsp[0].floatnumber), yyget_lineno());
}
#line 2105 "parser.cpp"
    break;

  case 94: /* UNARYEXP: UNARYOP EXP  */
#line 419 "parser.y"
                                  {
	(yyval.exp) = new AST::UnaryExp((yyvsp[-1].unaryop), (yyvsp[0].exp), yyget_lineno());
}
#line 2113 "parser.cpp"
    break;

  case 95: /* CALLEXP: IDEXP '(' EXPLIST ')'  */
#line 423 "parser.y"
                               {
	(yyval.exp) = new AST::CallExp((yyvsp[-3].idexp), (yyvsp[-1].explist), yyget_lineno());
}
#line 2121 "parser.cpp"
    break;

  case 96: /* UNARYOP: ADDOP  */
#line 427 "parser.y"
               {
	(yyval.unaryop) = AST::unaryop_t::ADD;
}
#line 2129 "parser.cpp"
    break;

  case 97: /* UNARYOP: SUBOP  */
#line 429 "parser.y"
          {
	(yyval.unaryop) = AST::unaryop_t::SUB;
}
#line 2137 "parser.cpp"
    break;

  case 98: /* UNARYOP: NOTOP  */
#line 431 "parser.y"
          {
	(yyval.unaryop) = AST::unaryop_t::NOT;
}
#line 2145 "parser.cpp"
    break;

  case 99: /* EXPLIST: %empty  */
#line 435 "parser.y"
          {
	(yyval.explist) = new std::vector<AST::Exp *>();
}
#line 2153 "parser.cpp"
    break;

  case 100: /* EXPLIST: EXP  */
#line 437 "parser.y"
        {
	(yyval.explist) = new std::vector<AST::Exp *>();
	(yyval.explist)->push_back((yyvsp[0].exp));
}
#line 2162 "parser.cpp"
    break;

  case 101: /* EXPLIST: EXPLIST ',' EXP  */
#line 440 "parser.y"
                    {
	(yyval.explist) = (yyvsp[-2].explist);
	(yyval.explist)->push_back((yyvsp[0].exp));
}
#line 2171 "parser.cpp"
    break;

  case 102: /* MULEXP: EXP MULOP EXP  */
#line 445 "parser.y"
                      {
	(yyval.exp) = new AST::MulExp((yyvsp[-2].exp), (yyvsp[-1].mul), (yyvsp[0].exp), yyget_lineno());
}
#line 2179 "parser.cpp"
    break;

  case 103: /* ADDEXP: EXP ADDOP EXP  */
#line 449 "parser.y"
                      {
	(yyval.exp) = new AST::AddExp((yyvsp[-2].exp), AST::add_t::ADD, (yyvsp[0].exp), yyget_lineno());
}
#line 2187 "parser.cpp"
    break;

  case 104: /* ADDEXP: EXP SUBOP EXP  */
#line 451 "parser.y"
                  {
	(yyval.exp) = new AST::AddExp((yyvsp[-2].exp), AST::add_t::SUB, (yyvsp[0].exp), yyget_lineno());
}
#line 2195 "parser.cpp"
    break;

  case 105: /* RELEXP: EXP RELOP EXP  */
#line 455 "parser.y"
                      {
	(yyval.exp) = new AST::RelExp((yyvsp[-2].exp), (yyvsp[-1].rel), (yyvsp[0].exp), yyget_lineno());
}
#line 2203 "parser.cpp"
    break;

  case 106: /* EQEXP: EXP EQUALOP EXP  */
#line 459 "parser.y"
                       {
	(yyval.exp) = new AST::EqualExp((yyvsp[-2].exp), (yyvsp[-1].equal), (yyvsp[0].exp), yyget_lineno());
}
#line 2211 "parser.cpp"
    break;

  case 107: /* LANDEXP: EXP LANDOP EXP  */
#line 463 "parser.y"
                        {
	(yyval.exp) = new AST::LAndExp((yyvsp[-2].exp), (yyvsp[0].exp), yyget_lineno());
}
#line 2219 "parser.cpp"
    break;

  case 108: /* LOREXP: EXP LOROP EXP  */
#line 467 "parser.y"
                      {
	(yyval.exp) = new AST::LOrExp((yyvsp[-2].exp), (yyvsp[0].exp), yyget_lineno());
}
#line 2227 "parser.cpp"
    break;

  case 109: /* GETINTEXP: GETINT '(' ')'  */
#line 471 "parser.y"
                          {
	(yyval.exp) = new AST::GetIntExp(yyget_lineno());
}
#line 2235 "parser.cpp"
    break;

  case 110: /* GETCHEXP: GETCH '(' ')'  */
#line 475 "parser.y"
                        {
	(yyval.exp) = new AST::GetChExp(yyget_lineno());
}
#line 2243 "parser.cpp"
    break;

  case 111: /* GETFLOATEXP: GETFLOAT '(' ')'  */
#line 479 "parser.y"
                              {
	(yyval.exp) = new AST::GetFloatExp(yyget_lineno());
}
#line 2251 "parser.cpp"
    break;

  case 112: /* GETARRAYEXP: GETARRAY '(' EXP ')'  */
#line 483 "parser.y"
                                  {
	(yyval.exp) = new AST::GetArrayExp((yyvsp[-1].exp), yyget_lineno());
}
#line 2259 "parser.cpp"
    break;

  case 113: /* GETFARRAYEXP: GETFARRAY '(' EXP ')'  */
#line 487 "parser.y"
                                    {
	(yyval.exp) = new AST::GetFArrayExp((yyvsp[-1].exp), yyget_lineno());
}
#line 2267 "parser.cpp"
    break;


#line 2271 "parser.cpp"

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

#line 492 "parser.y"
