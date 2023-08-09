#include "ast.h"
#include "LIR.h"

// using namespace AST;

LIR::Stm *ConstDecl::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitConstDeclNode(*this);
}

LIR::Stm *ConstDef::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitConstDefNode(*this);
}

LIR::Stm *VarDecl::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitVarDeclNode(*this);
}

LIR::Stm *VarDef::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitVarDefNode(*this);
}

LIR::Stm *FuncDef::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitFuncDefNode(*this);
}

LIR::Stm *Block::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitBlockNode(*this);
}

LIR::Stm *AssignStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitAssignStmtNode(*this);
}

LIR::Stm *ExpStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitExpStmtNode(*this);
}

LIR::Stm *IfStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitIfStmtNode(*this);
}

LIR::Stm *WhileStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitWhileStmtNode(*this);
}

LIR::Stm *BreakStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitBreakStmtNode(*this);
}

LIR::Stm *ContinueStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitContinueStmtNode(*this);
}

LIR::Stm *ReturnStmt::accept(LIR::ASTVisitor &visitor)
{

    return visitor.visitReturnStmtNode(*this);
}

LIR::BaseUnit LVal::accept(LIR::ASTVisitor &visitor)
{

    return visitor.visitLValStmtNode(*this);
}

LIR::BaseUnit IntNumber::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitIntNumberStmtNode(*this);
}

LIR::BaseUnit FloatNumber::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitFloatNumberNode(*this);
}

LIR::BaseUnit UnaryExp::accept(LIR::ASTVisitor &visitor)
{

    return visitor.visitUnaryExpNode(*this);
}

LIR::BaseUnit CallExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitCallExpNode(*this);
}

LIR::BaseUnit MulExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitMulExpNode(*this);
}

LIR::BaseUnit AddExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitAddExpExpNode(*this);
}

LIR::BaseUnit RelExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitRelExpNode(*this);
}

LIR::BaseUnit EqualExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitEqExpNode(*this);
}

LIR::BaseUnit LAndExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitLAndNode(*this);
}

LIR::BaseUnit LOrExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitLOrExpNode(*this);
}

LIR::BaseUnit IDExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitIDExpNode(*this);
}

LIR::BaseUnit Exp::calArray(LIR::ASTVisitor &visitor)
{
    return visitor.calArrayInitValsFromExp(*this);
}

LIR::BaseUnit ArrayInitVals::calArray(LIR::ASTVisitor &visitor)
{
    return visitor.calArrayInitValsFromArrayInitExp(*this);
}

LIR::BaseUnit ArrayInitVals::accept(LIR::ASTVisitor &visitor)
{
    assert(0);
    return LIR::BaseUnit();
}


LIR::BaseUnit GetIntExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitGetIntExpNode(*this);
}

LIR::BaseUnit GetChExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitGetChExpNode(*this);
}

LIR::BaseUnit GetFloatExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitGetFloatExpNode(*this);
}

LIR::BaseUnit GetArrayExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitGetArrayExpNode(*this);
}

LIR::BaseUnit GetFArrayExp::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitGetFArrayExpNode(*this);
}

LIR::Stm *PutIntStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitPutIntStmtNode(*this);
}

LIR::Stm *PutChStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitPutChStmtNode(*this);
}

LIR::Stm *PutFloatStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitPutFloatStmtNode(*this);
}


LIR::Stm *PutArrayStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitPutArrayStmtNode(*this);
}

LIR::Stm *PutFArrayStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitPutFArrayStmtNode(*this);
}

LIR::Stm *PutFStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitPutFStmtNode(*this);
}

LIR::Stm *StarttimeStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitStarttimStmt(*this);
}

LIR::Stm *StoptimeStmt::accept(LIR::ASTVisitor &visitor)
{
    return visitor.visitStoptimStmt(*this);
}
