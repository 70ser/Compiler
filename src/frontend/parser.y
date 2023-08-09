%{
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
%}

%union {
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
}

/* trans TOKEN to TYPES 
 * eg: MULOP -> mul -> AST::mul_t
*/
%token<token>               RETURN IF ELSE WHILE BREAK CONTINUE
%token<token>               VOIDT INTT FLOATT CONST
%token<token>               INTNUMBER
%token<token> 				GETINT GETCH GETFLOAT GETARRAY GETFARRAY
%token<token> 				PUTINT PUTCH PUTARRAY PUTFLOAT PUTFARRAY PUTF
%token<token> 				STARTTIME STOPTIME
%token<floatnumber>         FLOATNUMBER
%token<token>               ADDOP SUBOP NOTOP LANDOP LOROP
%token<mul>                 MULOP
%token<equal>               EQUALOP
%token<rel>                 RELOP
%token<id>                  ID

/* trans syntactic unit to AST-defined Class
 * eg: DECL -> decl -> AST::Decl
*/
%type <root>                	COMPROOT
%type <compunit>            	COMPUNIT
%type <decl>                	DECL
%type <constdecl>           	CONSTDECL
%type <btype>               	BTYPE
%type <constdef>            	CONSTDEF
%type <vardecl>              	VARDECL
%type <vardef>              	VARDEF
%type <initval>             	INITVAL
%type <arrayinitialvals>    	ARRAYINITIALVALS
%type <funcdef>             	FUNCDEF
%type <parameter>           	PARAMETER
%type <block>               	BLOCK
%type <blockitem>           	BLOCKITEM
%type <stmt>                	STMT IFSTMT WHILESTMT ASSIGNSTMT EXPSTMT BREAKSTMT CONTINUESTMT RETURNSTMT
%type <exp>                 	EXP PRIMARYEXP UNARYEXP MULEXP ADDEXP RELEXP EQEXP LANDEXP LOREXP CALLEXP
%type <exp> 					GETINTEXP GETCHEXP GETFLOATEXP GETARRAYEXP GETFARRAYEXP
%type <idexp>               	IDEXP
%type <stmt> 					PUTINTSTMT PUTCHSTMT PUTARRAYSTMT PUTFLOATSTMT PUTFARRAYSTMT PUTFSTMT
%type <stmt> 					STARTTIMESTMT STOPTIMESTMT
%type <number>              	NUMBER
%type <unaryop>             	UNARYOP
%type <lval>                	LVAL
%type <constdeflist>			CONSTDEFLIST
%type <vardeflist> 				VARDEFLIST
%type <arrayindexlist> 			ARRAYINDEXLIST
%type <initvallist> 			INITVALLIST
%type <parameters> 				PARAMETERS
%type <blockitemlist> 			BLOCKITEMLIST
%type <explist> 				EXPLIST



%left                       LOROP
%left                       LANDOP
%left                       EQUALOP
%left                       RELOP
%left                       ADDOP SUBOP
%left                       MULOP
%left                       prec1   // handle unaryop/if/else
%left                       prec2
%left                       ELSE


%start COMPROOT

%%

COMPROOT: COMPROOT COMPUNIT {
    $$ = $1;
    $$->comp_list.push_back($2);
    root = $$;
} | COMPUNIT {
    $$ = new AST::CompRoot(yyget_lineno());
    $$->comp_list.push_back($1);
    root = $$;
}

COMPUNIT: DECL  {
	$$ = static_cast<AST::CompUnit *>($1);
} | FUNCDEF {
	$$ = static_cast<AST::CompUnit *>($1);
}

/* ========================= DECL ========================= */

DECL: CONSTDECL {
	$$ = static_cast<AST::Decl *>($1);
} | VARDECL {
	$$ = static_cast<AST::Decl *>($1);
}

CONSTDECL: CONST BTYPE CONSTDEFLIST ';' {
	$$ = new AST::ConstDecl($2, $3, yyget_lineno());
}

CONSTDEFLIST: CONSTDEF {
	$$ = new std::vector<AST::ConstDef*>();
	$$->push_back($1);
} | CONSTDEFLIST ',' CONSTDEF {
	$$ = $1;
	$1->push_back($3);
}

BTYPE: INTT {
	$$ = AST::base_t::INT;
} | FLOATT {
	$$ = AST::base_t::FLOAT;
} | VOIDT {
	$$ = AST::base_t::VOID;
}

CONSTDEF: IDEXP ARRAYINDEXLIST '=' INITVAL {
	$$ = new AST::ConstDef($1, $2, $4, yyget_lineno());
}

VARDECL: BTYPE VARDEFLIST ';' {
	$$ = new AST::VarDecl($1, $2, yyget_lineno());
}

VARDEFLIST: VARDEF {
	$$ = new std::vector<AST::VarDef*>();
	$$->push_back($1);
} | VARDEFLIST ',' VARDEF {
	$$ = $1;
	$1->push_back($3);
}

VARDEF: IDEXP ARRAYINDEXLIST {
	$$ = new AST::VarDef($1, $2, nullptr, yyget_lineno());
} | IDEXP ARRAYINDEXLIST '=' INITVAL {
	$$ = new AST::VarDef($1, $2, $4, yyget_lineno());
}


ARRAYINDEXLIST:  {
	$$ = new std::vector<AST::Exp*>();
} | ARRAYINDEXLIST '[' EXP ']' {
	$$ = $1;
	$$->push_back($3);
}

INITVAL: EXP {
	$$ = $1;
} | ARRAYINITIALVALS {
	$$ = $1;
}


ARRAYINITIALVALS : '{' INITVALLIST '}' {
	$$ = new AST::ArrayInitVals(yyget_lineno(), $2);
}

INITVALLIST: {
	$$ = new std::vector<AST::InitVal *>();
} | INITVAL {
	$$ = new std::vector<AST::InitVal *>();
    $$->push_back($1);
} | INITVALLIST ',' INITVAL {
	$$ = $1;
	$$->push_back($3);
}



/* ========================= FUNC DEF ========================= */

FUNCDEF: BTYPE IDEXP '(' PARAMETERS ')' BLOCK {
	$$ = new AST::FuncDef($1, $2, $4, $6, yyget_lineno());
}

PARAMETERS:  {
	$$ = new std::vector<AST::Parameter*>();
} | PARAMETER {
    $$ = new std::vector<AST::Parameter*>();
    $$->push_back($1);
} | PARAMETERS ',' PARAMETER {
	$$ = $1;
	$$->push_back($3);
}

PARAMETER: BTYPE IDEXP {
	$$ = new AST::Parameter($1, $2, new std::vector<AST::Exp *>(), yyget_lineno());
} | BTYPE IDEXP '[' ']' ARRAYINDEXLIST {
    // int a[][3]: define 1's demension as 1
	std::vector<AST::Exp *> *x = new std::vector<AST::Exp *>();
	x->push_back(new AST::IntNumber(1, yyget_lineno()));
	for (const auto &y : *$5) {
		x->push_back(y);
	}
	$$ = new AST::Parameter($1, $2, x, yyget_lineno());
}

BLOCK: '{' BLOCKITEMLIST '}' {
	$$ = new AST::Block($2, yyget_lineno());
}

BLOCKITEMLIST:  {
	$$ = new std::vector<AST::BlockItem *>();
} | BLOCKITEMLIST BLOCKITEM {
	$$ = $1;
	$$->push_back($2);
}

BLOCKITEM: DECL {
	$$ = $1;
} | STMT {
	$$ = $1;
}

STMT: ASSIGNSTMT ';' {
	$$ = $1;
} | ';' {
	$$ = new AST::ExpStmt(new AST::IntNumber(0, yyget_lineno()), yyget_lineno());
} | EXPSTMT ';' {
	$$ = $1;
} | BLOCK {
	$$ = $1;
} | IFSTMT {
	$$ = $1;
} | WHILESTMT {
	$$ = $1;
} | BREAKSTMT ';' {
	$$ = $1;
} | CONTINUESTMT ';' {
	$$ = $1;
} | RETURNSTMT ';' {
	$$ = $1;
} | PUTINTSTMT ';' {
	$$ = $1;
} | PUTCHSTMT ';' {
	$$ = $1;
} | PUTARRAYSTMT ';' {
	$$ = $1;
} | PUTFLOATSTMT ';' {
	$$ = $1;
} | PUTFARRAYSTMT ';' {
	$$ = $1;
} | PUTFSTMT ';' {
	$$ = $1;
} | STARTTIMESTMT ';' {
	$$ = $1;
} | STOPTIMESTMT ';' {
	$$ = $1;
}

ASSIGNSTMT: LVAL '=' EXP {
	$$ = new AST::AssignStmt($1, $3, yyget_lineno());
}

EXPSTMT: EXP {
	$$ = new AST::ExpStmt($1, yyget_lineno());
}

IFSTMT: IF '(' EXP ')' STMT %prec prec2 {
	$$ = new AST::IfStmt($3, $5, nullptr, yyget_lineno());
} | IF '(' EXP ')' STMT ELSE STMT {
	$$ = new AST::IfStmt($3, $5, $7, yyget_lineno());
}

WHILESTMT: WHILE '(' EXP ')' STMT {
	$$ = new AST::WhileStmt($3, $5, yyget_lineno());
}

BREAKSTMT: BREAK {
	$$ = new AST::BreakStmt(yyget_lineno());
}

CONTINUESTMT: CONTINUE {
	$$ = new AST::ContinueStmt(yyget_lineno());
}

RETURNSTMT: RETURN {
	$$ = new AST::ReturnStmt(nullptr, yyget_lineno());
} | RETURN EXP {
	$$ = new AST::ReturnStmt($2, yyget_lineno());
}

PUTINTSTMT: PUTINT '(' EXP ')' {
	$$ = new AST::PutIntStmt($3, yyget_lineno());
}

PUTCHSTMT: PUTCH '(' EXP ')' {
	$$ = new AST::PutChStmt($3, yyget_lineno());
}

PUTFLOATSTMT: PUTFLOAT '(' EXP ')' {
	$$ = new AST::PutFloatStmt($3, yyget_lineno());
}

PUTARRAYSTMT: PUTARRAY '(' EXP ',' EXP ')' {
	$$ = new AST::PutArrayStmt($3, $5, yyget_lineno());
}

PUTFARRAYSTMT: PUTFARRAY '(' EXP ',' EXP ')' {
	$$ = new AST::PutFArrayStmt($3, $5, yyget_lineno());
}

PUTFSTMT: PUTF '(' EXPLIST ')' {
	$$ = new AST::PutFStmt($3, yyget_lineno());
}

STARTTIMESTMT: STARTTIME {
	$$ = new AST::StarttimeStmt(yyget_lineno());
}

STOPTIMESTMT: STOPTIME {
	$$ = new AST::StoptimeStmt(yyget_lineno());
}

EXP: PRIMARYEXP {
	$$ = $1;
} | UNARYEXP {
	$$ = $1;
} | MULEXP {
	$$ = $1;
} | ADDEXP {
	$$ = $1;
} | RELEXP {
	$$ = $1;
} | EQEXP {
	$$ = $1;
} | LANDEXP {
	$$ = $1;
} | LOREXP {
	$$ = $1;
} | CALLEXP {
	$$ = $1;
} | GETINTEXP {
	$$ = $1;
} | GETCHEXP {
	$$ = $1;
} | GETFLOATEXP {
	$$ = $1;
} | GETARRAYEXP {
	$$ = $1;
} | GETFARRAYEXP {
	$$ = $1;
}

IDEXP : ID {
    $$ = new AST::IDExp($1, yyget_lineno());
}

LVAL: IDEXP ARRAYINDEXLIST {
	$$ = new AST::LVal($1, $2, yyget_lineno());
}

PRIMARYEXP: '(' EXP ')' {
	$$ = $2;
} | LVAL {
	$$ = $1;
} | NUMBER {
	$$ = $1;
}

NUMBER: INTNUMBER {
	$$ = new AST::IntNumber($1, yyget_lineno());
} | FLOATNUMBER {
	$$ = new AST::FloatNumber($1, yyget_lineno());
}

UNARYEXP: UNARYOP EXP %prec prec1 {
	$$ = new AST::UnaryExp($1, $2, yyget_lineno());
} 

CALLEXP: IDEXP '(' EXPLIST ')' {
	$$ = new AST::CallExp($1, $3, yyget_lineno());
}

UNARYOP: ADDOP {
	$$ = AST::unaryop_t::ADD;
} | SUBOP {
	$$ = AST::unaryop_t::SUB;
} | NOTOP {
	$$ = AST::unaryop_t::NOT;
}

EXPLIST:  {
	$$ = new std::vector<AST::Exp *>();
} | EXP {
	$$ = new std::vector<AST::Exp *>();
	$$->push_back($1);
} | EXPLIST ',' EXP {
	$$ = $1;
	$$->push_back($3);
}

MULEXP: EXP MULOP EXP {
	$$ = new AST::MulExp($1, $2, $3, yyget_lineno());
}

ADDEXP: EXP ADDOP EXP {
	$$ = new AST::AddExp($1, AST::add_t::ADD, $3, yyget_lineno());
} | EXP SUBOP EXP {
	$$ = new AST::AddExp($1, AST::add_t::SUB, $3, yyget_lineno());
}

RELEXP: EXP RELOP EXP {
	$$ = new AST::RelExp($1, $2, $3, yyget_lineno());
}

EQEXP: EXP EQUALOP EXP {
	$$ = new AST::EqualExp($1, $2, $3, yyget_lineno());
}

LANDEXP: EXP LANDOP EXP {
	$$ = new AST::LAndExp($1, $3, yyget_lineno());
}

LOREXP: EXP LOROP EXP {
	$$ = new AST::LOrExp($1, $3, yyget_lineno());
}

GETINTEXP: GETINT '(' ')' {
	$$ = new AST::GetIntExp(yyget_lineno());
}

GETCHEXP: GETCH '(' ')' {
	$$ = new AST::GetChExp(yyget_lineno());
}

GETFLOATEXP: GETFLOAT '(' ')' {
	$$ = new AST::GetFloatExp(yyget_lineno());
}

GETARRAYEXP: GETARRAY '(' EXP ')' {
	$$ = new AST::GetArrayExp($3, yyget_lineno());
}

GETFARRAYEXP: GETFARRAY '(' EXP ')' {
	$$ = new AST::GetFArrayExp($3, yyget_lineno());
}


%%