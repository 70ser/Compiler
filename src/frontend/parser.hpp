/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    RETURN = 258,                  /* RETURN  */
    IF = 259,                      /* IF  */
    ELSE = 260,                    /* ELSE  */
    WHILE = 261,                   /* WHILE  */
    BREAK = 262,                   /* BREAK  */
    CONTINUE = 263,                /* CONTINUE  */
    VOIDT = 264,                   /* VOIDT  */
    INTT = 265,                    /* INTT  */
    FLOATT = 266,                  /* FLOATT  */
    CONST = 267,                   /* CONST  */
    INTNUMBER = 268,               /* INTNUMBER  */
    GETINT = 269,                  /* GETINT  */
    GETCH = 270,                   /* GETCH  */
    GETFLOAT = 271,                /* GETFLOAT  */
    GETARRAY = 272,                /* GETARRAY  */
    GETFARRAY = 273,               /* GETFARRAY  */
    PUTINT = 274,                  /* PUTINT  */
    PUTCH = 275,                   /* PUTCH  */
    PUTARRAY = 276,                /* PUTARRAY  */
    PUTFLOAT = 277,                /* PUTFLOAT  */
    PUTFARRAY = 278,               /* PUTFARRAY  */
    PUTF = 279,                    /* PUTF  */
    STARTTIME = 280,               /* STARTTIME  */
    STOPTIME = 281,                /* STOPTIME  */
    FLOATNUMBER = 282,             /* FLOATNUMBER  */
    ADDOP = 283,                   /* ADDOP  */
    SUBOP = 284,                   /* SUBOP  */
    NOTOP = 285,                   /* NOTOP  */
    LANDOP = 286,                  /* LANDOP  */
    LOROP = 287,                   /* LOROP  */
    MULOP = 288,                   /* MULOP  */
    EQUALOP = 289,                 /* EQUALOP  */
    RELOP = 290,                   /* RELOP  */
    ID = 291,                      /* ID  */
    prec1 = 292,                   /* prec1  */
    prec2 = 293                    /* prec2  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "parser.y"

    int                     		token;
    std::string             		*id;
    std::string             		*floatnumber;
    AST::mul_t              		mul;
    AST::rel_t              		rel;
    AST::equal_t            		equal;

    AST::CompRoot           		*root;
    AST::CompUnit           		*compunit;

    AST::Decl               		*decl;
    AST::ConstDecl          		*constdecl;
    AST::base_t             		btype;
    AST::ConstDef           		*constdef;
	std::vector<AST::ConstDef*>		*constdeflist;
    AST::VarDecl            		*vardecl;
	std::vector<AST::VarDef*> 		*vardeflist;
    AST::VarDef             		*vardef;
	std::vector<AST::Exp*> 			*arrayindexlist;

    AST::InitVal            		*initval;
    AST::ArrayInitVals   			*arrayinitialvals;
	std::vector<AST::InitVal*> 		*initvallist;

    AST::FuncDef            		*funcdef;
	std::vector<AST::Parameter*> 	*parameters;
    AST::Parameter          		*parameter;
    AST::Block              		*block;
	std::vector<AST::BlockItem*> 	*blockitemlist;
    AST::BlockItem          		*blockitem;
    AST::Stmt               		*stmt;
    AST::Exp                		*exp;
	AST::IDExp              		*idexp;
	std::vector<AST::Exp*> 			*explist;
    AST::Number             		*number;
    AST::LVal               		*lval;

    AST::unaryop_t          		unaryop;

#line 143 "parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
