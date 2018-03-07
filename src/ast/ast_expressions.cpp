#include "../../include/ast/ast_expressions.hpp"

//-----------------------------------------------------------------------------

ExpressionVariable::ExpressionVariable(const std::string* _variable) : variable(_variable){}

void ExpressionVariable::print_c(std::ostream &dst) const {
	dst << *variable;
}

void ExpressionVariable::py_translate(std::ostream &dst) const {
	dst << *variable;
}

void ExpressionVariable::print_mips(std::ostream &dst) const {
	dst << *variable;
}

//-----------------------------------------------------------------------------


Value::Value(const double* _number) : number(_number){}

void Value::print_c(std::ostream &dst) const {
	dst << *number;
}

void Value::py_translate(std::ostream &dst) const {
	dst << *number;
}

void Value::print_mips(std::ostream &dst) const {
	dst << *number;
}

//-----------------------------------------------------------------------------


MultExpression::MultExpression(const NodePtr _lhs, const NodePtr _rhs) : lhs(_lhs),rhs(_rhs){}

void MultExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "*";
	rhs->print_c(dst);
}

void MultExpression::py_translate(std::ostream &dst) const {
	lhs->py_translate(dst);
	dst << "*";
	rhs->py_translate(dst);
}

void MultExpression::print_mips(std::ostream &dst) const {}


//-----------------------------------------------------------------------------


AssignExpr::AssignExpr(const NodePtr _unaryexpr, const NodePtr _assignop,
 const NodePtr _assignexpr) : unaryexpr(_unaryexpr), assignop(_assignop),
	assignexpr(_assignexpr){}

void AssignExpr::print_c(std::ostream &dst) const {
	std::cerr << "assignexpr";
	unaryexpr->print_c(dst);
	dst << " ";
	assignop->print_c(dst);
	dst << " ";
	assignexpr->print_c(dst);
}

void AssignExpr::py_translate(std::ostream &dst) const {
	unaryexpr->py_translate(dst);
	dst << "";
	assignop->py_translate(dst);
	dst << " ";
	assignexpr->py_translate(dst);
}

void AssignExpr::print_mips(std::ostream &dst) const {}

//-----------------------------------------------------------------------------

AddExpression::AddExpression(const NodePtr _lhs, const NodePtr _rhs) : lhs(_lhs),rhs(_rhs){}

void AddExpression::print_c(std::ostream &dst) const {
	std::cerr << "addexpr";
	lhs->print_c(dst);
	dst << "+";
	rhs->print_c(dst);
}

void AddExpression::py_translate(std::ostream &dst) const {
	lhs->py_translate(dst);
	dst << "+";
	rhs->py_translate(dst);
}

void AddExpression::print_mips(std::ostream &dst) const {}

//-----------------------------------------------------------------------------


SubExpression::SubExpression(const NodePtr _lhs, const NodePtr _rhs) : lhs(_lhs),rhs(_rhs){}

void SubExpression::print_c(std::ostream &dst) const {
	std::cerr << "subexpr";
	lhs->print_c(dst);
	dst << "-";
	rhs->print_c(dst);
}

void SubExpression::py_translate(std::ostream &dst) const {
	lhs->py_translate(dst);
	dst << "-";
	rhs->py_translate(dst);
}

void SubExpression::print_mips(std::ostream &dst) const {}


//-----------------------------------------------------------------------------

//Reduce this by inheriting lhs and rhs??
ORExpression::ORExpression(const NodePtr _lhs, const NodePtr _rhs) : lhs(_lhs),rhs(_rhs){}
void ORExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "||";
	rhs->print_c(dst);
}
void ORExpression::py_translate(std::ostream &dst) const {
	lhs->py_translate(dst);
	dst << " or ";
	rhs->py_translate(dst);
}
void ORExpression::print_mips(std::ostream &dst) const {}

//-----------------------------------------------------------------------------


ANDExpression::ANDExpression(const NodePtr _lhs, const NodePtr _rhs) : lhs(_lhs),rhs(_rhs){}

void ANDExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "&&";
	rhs->print_c(dst);
}

void ANDExpression::py_translate(std::ostream &dst) const {
	lhs->py_translate(dst);
	dst << " and ";
	rhs->py_translate(dst);
}

void ANDExpression::print_mips(std::ostream &dst) const {}

//-----------------------------------------------------------------------------

LessThanExpression::LessThanExpression(const NodePtr _lhs, const NodePtr _rhs) : lhs(_lhs),rhs(_rhs){}

void LessThanExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "<";
	rhs->print_c(dst);
}

void LessThanExpression::py_translate(std::ostream &dst) const {
	lhs->py_translate(dst);
	dst << "<";
	rhs->py_translate(dst);
}

void LessThanExpression::print_mips(std::ostream &dst) const {}

//-----------------------------------------------------------------------------


GreaterThanExpression::GreaterThanExpression(const NodePtr _lhs, const NodePtr _rhs) : lhs(_lhs),rhs(_rhs){}

void GreaterThanExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << ">";
	rhs->print_c(dst);
}

void GreaterThanExpression::py_translate(std::ostream &dst) const {
	lhs->py_translate(dst);
	dst << ">";
	rhs->py_translate(dst);
}

void GreaterThanExpression::print_mips(std::ostream &dst) const {}


//-----------------------------------------------------------------------------


EqualityExpression::EqualityExpression(const NodePtr _lhs, const NodePtr _rhs) : lhs(_lhs),rhs(_rhs){}

void EqualityExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "==";
	rhs->print_c(dst);
}

void EqualityExpression::py_translate(std::ostream &dst) const {
	lhs->py_translate(dst);
	dst << "==";
	rhs->py_translate(dst);
}

void EqualityExpression::print_mips(std::ostream &dst) const {}
//-----------------------------------------------------------------------------


LonePostfixExpression::LonePostfixExpression(const NodePtr _expr) : expr(_expr){}

void LonePostfixExpression::print_c(std::ostream &dst) const {
	expr->print_c(dst);
	dst << "()";
}

void LonePostfixExpression::py_translate(std::ostream &dst) const {
	expr->py_translate(dst);
	dst << "()";
}

void LonePostfixExpression::print_mips(std::ostream &dst) const {}

//-----------------------------------------------------------------------------


PostfixArguExpression::PostfixArguExpression(const NodePtr _expr, const NodePtr _arguexpr):
expr(_expr), arguexpr(_arguexpr){}

void PostfixArguExpression::print_c(std::ostream &dst) const {
	expr->print_c(dst);
	dst << "(";
	arguexpr->print_c(dst);
	dst << ")";
}

void PostfixArguExpression::py_translate(std::ostream &dst) const {
	expr->py_translate(dst);
	dst << "(";
	arguexpr->py_translate(dst);
	dst << ")";
}

void PostfixArguExpression::print_mips(std::ostream &dst) const {}

//-----------------------------------------------------------------------------

AssignExprList::AssignExprList(const NodePtr _exprlist, const NodePtr _expr):
	exprlist(_exprlist), expr(_expr){}

void AssignExprList::print_c(std::ostream &dst) const {
	exprlist->print_c(dst);
	dst << ",";
	expr->print_c(dst);
}

void AssignExprList::py_translate(std::ostream &dst) const {
	exprlist->py_translate(dst);
	dst << ",";
	expr->py_translate(dst);
}

void AssignExprList::print_mips(std::ostream &dst) const {}

//-----------------------------------------------------------------------------

UnaryOpExpr::UnaryOpExpr(const NodePtr _op, const NodePtr _expr) : op(_op),expr(_expr){}

void UnaryOpExpr::print_c(std::ostream &dst) const {
	op->print_c(dst);
	expr->print_c(dst);
}

void UnaryOpExpr::py_translate(std::ostream &dst) const {
	op->print_c(dst);
	expr->py_translate(dst);
}

void UnaryOpExpr::print_mips(std::ostream &dst) const {}
//-----------------------------------------------------------------------------

UnaryOp::UnaryOp(const std::string _op) : op(_op){}

void UnaryOp::print_c(std::ostream &dst) const {
	dst << op;
}

void UnaryOp::py_translate(std::ostream &dst) const {
	dst << op;
}

void UnaryOp::print_mips(std::ostream &dst) const {}

//-----------------------------------------------------------------------------