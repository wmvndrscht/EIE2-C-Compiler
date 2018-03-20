#include "../../include/ast/ast_expressions.hpp"

//-----------------------------------------------------------------------------

ExpressionVariable::ExpressionVariable(const std::string* _variable) : variable(_variable){}

void ExpressionVariable::print_c(std::ostream &dst) const {
	dst << *variable;
}

void ExpressionVariable::py_translate(std::ostream &dst, const scope &scp) const {
	dst << *variable;
}

void ExpressionVariable::print_mips(std::ostream &dst, context &program) const {
	// dst << *variable;

	if( program.isVarGlobal(*variable) ){
		dst << "\tlui $" << program.getnReg() << ",%hi(" << *variable << ")\n";
		dst << "\tlw $" << program.getnReg() << ",%lo(" << *variable << ")($" << program.getnReg() << ")\n";
	}
	else{
		dst << "\tlw $" << program.getnReg() << "," << program.getFrameSize() - program.getVarOffset(*variable) << "($fp)" << "\n";
	}
	// dst << "\t" << "lw $fp," << FrameSize-4 << "($sp)\n";
}

std::string ExpressionVariable::get_ID() const{
	return *variable;
}

//-----------------------------------------------------------------------------


Value::Value(const double* _number) : number(_number){}

void Value::print_c(std::ostream &dst) const {
	dst << *number;
}

void Value::py_translate(std::ostream &dst, const scope &scp) const {
	dst << *number;
}

void Value::print_mips(std::ostream &dst, context &program) const {
	if(program.getScopeNum() == 0){
		dst << *number;
	}
	else{
		dst << "\tli $" << program.getnReg() << "," << *number <<"\n";
	}
}

std::string Value::get_ID() const{
	throw std::runtime_error("Value class should not have ID");
	return "Value should not have ID";
}


//-----------------------------------------------------------------------------


MultExpression::MultExpression(const Expression* _lhs, const Expression* _rhs) : lhs(_lhs),rhs(_rhs){}

void MultExpression::print_c(std::ostream &dst) const {
	dst << "(";
	lhs->print_c(dst);
	dst << ")*(";
	rhs->print_c(dst);
	dst << ")";
}

void MultExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "*";
	rhs->py_translate(dst,scp);
}

void MultExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();

	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tmultu $"  << dReg <<
		",$" << program.getnReg() << "\n";
	dst << "\tmflo $" << dReg << "\n";
	program.freeReg();
}


//-----------------------------------------------------------------------------


// AssignEqExpr::AssignEqExpr(const Expression* _unaryexpr, const Declaration* _assignop,
//  const Expression* _assignexpr) : unaryexpr(_unaryexpr), assignop(_assignop),
// 	assignexpr(_assignexpr){}

AssignEqExpr::AssignEqExpr(const Expression* _unaryexpr,const Expression* _assignexpr) :
 unaryexpr( _unaryexpr), assignexpr(_assignexpr){}

void AssignEqExpr::print_c(std::ostream &dst) const {
	unaryexpr->print_c(dst);
	dst << " = ";
	assignexpr->print_c(dst);
}

void AssignEqExpr::py_translate(std::ostream &dst, const scope &scp) const {
	unaryexpr->py_translate(dst,scp);
	dst << "= ";
	assignexpr->py_translate(dst,scp);
}

void AssignEqExpr::print_mips(std::ostream &dst, context &program) const {
	//load value into getnReg


	unaryexpr->print_mips(dst,program);
	int destReg = program.getnReg();
	Identify* a = (Identify*)unaryexpr;

	std::string name = a->get_ID();
	// std::string name = "a";
	//assume operator is "="
	//evaluate expression in next Register
	program.assignReg();
	//if variable is global
	assignexpr->print_mips(dst,program);
	if( program.isVarGlobal(name) ){
		dst << "\tlui $" << destReg << ",%hi(" << name << ")\n";
		dst << "\tsw $" << program.getnReg() << ",%lo(" << name << ")($" << destReg << ")\n";

	}
	else{
		
		dst << "\tmove $" << destReg << ",$" << program.getnReg() << "\n";
		dst << "\tsw $" << destReg << ","  << program.getFrameSize()- program.getVarOffset(name) << "($fp)\n";
	}
	program.freeReg();
}

//-----------------------------------------------------------------------------

AddExpression::AddExpression(const Expression* _lhs, const Expression* _rhs) : lhs(_lhs),rhs(_rhs){}

void AddExpression::print_c(std::ostream &dst) const {
	dst << "(";
	lhs->print_c(dst);
	dst << ")+(";
	rhs->print_c(dst);
	dst << ")";
}

void AddExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "+";
	rhs->py_translate(dst,scp);
}

void AddExpression::print_mips(std::ostream &dst, context &program) const {
	//evaluate expression
	//pass value into destReg
	int dReg = program.getnReg();
	//Run lhs where value is stored into lReg
	lhs->print_mips(dst,program);
	//Adjust availReg for rhs
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\taddu $" << dReg << ",$" << dReg <<
		",$" << program.getnReg() << "\n";
	program.freeReg();

}

//-----------------------------------------------------------------------------


SubExpression::SubExpression(const Expression* _lhs, const Expression* _rhs) : lhs(_lhs),rhs(_rhs){}

void SubExpression::print_c(std::ostream &dst) const {
	dst << "(";
	lhs->print_c(dst);
	dst << ")-(";
	rhs->print_c(dst);
	dst << ")";
}

void SubExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "-";
	rhs->py_translate(dst,scp);
}

void SubExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	//Run lhs where value is stored into lReg
	lhs->print_mips(dst,program);
	//Adjust availReg for rhs
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tsubu $" << dReg << ",$" << dReg <<
		",$" << program.getnReg() << "\n";
	program.freeReg();
}


//-----------------------------------------------------------------------------

//Reduce this by inheriting lhs and rhs??
ORExpression::ORExpression(const Expression* _lhs, const Expression* _rhs) : lhs(_lhs),rhs(_rhs){}
void ORExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "||";
	rhs->print_c(dst);
}
void ORExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << " or ";
	rhs->py_translate(dst,scp);
}
void ORExpression::print_mips(std::ostream &dst, context &program) const {}

//-----------------------------------------------------------------------------


ANDExpression::ANDExpression(const Expression* _lhs, const Expression* _rhs) : lhs(_lhs),rhs(_rhs){}

void ANDExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "&&";
	rhs->print_c(dst);
}

void ANDExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << " and ";
	rhs->py_translate(dst,scp);
}

void ANDExpression::print_mips(std::ostream &dst, context &program) const {}

//-----------------------------------------------------------------------------

LessThanExpression::LessThanExpression(const Expression* _lhs, const Expression* _rhs) : lhs(_lhs),rhs(_rhs){}

void LessThanExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "<";
	rhs->print_c(dst);
}

void LessThanExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "<";
	rhs->py_translate(dst,scp);
}

void LessThanExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tslt $" << dReg << ",$" << dReg <<
		",$" <<  program.getnReg() << "\n";
	program.freeReg();
}

//-----------------------------------------------------------------------------


GreaterThanExpression::GreaterThanExpression(const Expression* _lhs, const Expression* _rhs) : lhs(_lhs),rhs(_rhs){}

void GreaterThanExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << ">";
	rhs->print_c(dst);
}

void GreaterThanExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << ">";
	rhs->py_translate(dst,scp);
}

void GreaterThanExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tslt $" << dReg << ",$" << program.getnReg() <<
		",$" <<  dReg << "\n";
	program.freeReg();
}


//-----------------------------------------------------------------------------


EqualityExpression::EqualityExpression(const Expression* _lhs, const Expression* _rhs) : lhs(_lhs),rhs(_rhs){}

void EqualityExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "==";
	rhs->print_c(dst);
}

void EqualityExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "==";
	rhs->py_translate(dst,scp);
}

void EqualityExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	std::string eq = program.createLabel();
	std::string end = program.createLabel();

	//Run lhs where value is stored into lReg
	lhs->print_mips(dst,program);
	//Adjust availReg for rhs
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tbeq $" << dReg << ",$" << program.getnReg() <<
		"," << eq << "\n";
	dst << "\tnop\n";
	dst << "\tli $" << dReg << ",0\n";
	dst << "\tb " << end << "\n";
	dst << eq << ":\n";
	dst << "\tli $" << dReg << ",1\n";
	dst << end <<":\n";


	program.freeReg();
}
//-----------------------------------------------------------------------------


LonePostfixExpression::LonePostfixExpression(const Identify* _expr) : expr(_expr){}

void LonePostfixExpression::print_c(std::ostream &dst) const {
	expr->print_c(dst);
	dst << "()";
}

void LonePostfixExpression::py_translate(std::ostream &dst, const scope &scp) const {
	expr->py_translate(dst,scp);
	dst << "()";
}

void LonePostfixExpression::print_mips(std::ostream &dst, context &program) const {
	std::string name = expr->get_ID();
	dst << "\tjal " << name << "\n";
	dst << "\tnop\n"; 
	dst << "\tmove $" << program.getnReg() << ",$2\n";
}

std::string LonePostfixExpression::get_ID() const{
	return expr->get_ID();
}

//-----------------------------------------------------------------------------


PostfixArguExpression::PostfixArguExpression(const Identify* _expr, const Expression* _arguexpr):
expr(_expr), arguexpr(_arguexpr){}

void PostfixArguExpression::print_c(std::ostream &dst) const {
	expr->print_c(dst);
	dst << "(";
	arguexpr->print_c(dst);
	dst << ")";
}

void PostfixArguExpression::py_translate(std::ostream &dst, const scope &scp) const {
	expr->py_translate(dst,scp);
	dst << "(";
	arguexpr->py_translate(dst,scp);
	dst << ")";
}

void PostfixArguExpression::print_mips(std::ostream &dst, context &program) const {

	std::string name = expr->get_ID();

	program.resetParamPass();
	arguexpr->print_mips(dst,program);
	dst << "\tjal " << name << "\n";
	dst << "\tnop\n";
	dst << "\tmove $" << program.getnReg() << ",$2\n";
}

std::string PostfixArguExpression::get_ID() const{
	return expr->get_ID();
}

//-----------------------------------------------------------------------------

AssignExprList::AssignExprList(const Expression* _exprlist, const Expression* _expr):
	exprlist(_exprlist), expr(_expr){}

void AssignExprList::print_c(std::ostream &dst) const {
	if(exprlist != NULL){
		exprlist->print_c(dst);
		dst << ",";
	}
	expr->print_c(dst);
}

void AssignExprList::py_translate(std::ostream &dst, const scope &scp) const {
	if(exprlist != NULL){
		exprlist->py_translate(dst,scp);
		dst << ",";
	}
	expr->py_translate(dst,scp);
}

void AssignExprList::print_mips(std::ostream &dst, context &program) const {
	//null don't print next expr
	//in postfixarguexpression set param to 0
	if(exprlist != NULL){
		exprlist->print_mips(dst,program);
	}
	expr->print_mips(dst,program);
	int reg = 4 + program.get_ParamPass();
	dst << "\tmove $" << reg << ",$" << program.getdestReg() << "\n";
	program.incrParamPass();
}

//-----------------------------------------------------------------------------

UnaryOpExpr::UnaryOpExpr(const Expression* _op, const Expression* _expr) : op(_op),expr(_expr){}

void UnaryOpExpr::print_c(std::ostream &dst) const {
	dst << "[";
	op->print_c(dst);
	dst << "]{";
	expr->print_c(dst);
	dst << "}";
}

void UnaryOpExpr::py_translate(std::ostream &dst, const scope &scp) const {
	op->print_c(dst);
	expr->py_translate(dst,scp);
}

void UnaryOpExpr::print_mips(std::ostream &dst, context &program) const {}

std::string UnaryOpExpr::get_ID() const{
	return "yet to support UnaryOpExpr class get_ID()";
}


//-----------------------------------------------------------------------------

UnaryOp::UnaryOp(const std::string _op) : op(_op){}

void UnaryOp::print_c(std::ostream &dst) const {
	dst << op;
}

void UnaryOp::py_translate(std::ostream &dst, const scope &scp) const {
	dst << op;
}

void UnaryOp::print_mips(std::ostream &dst, context &program) const {}



//-----------------------------------------------------------------------------

ParenExpr::ParenExpr(const Expression* _expr) : expr(_expr){}

void ParenExpr::print_c(std::ostream &dst) const {
	dst << "(";
	expr->print_c(dst);
	dst << ")";
}

void ParenExpr::py_translate(std::ostream &dst, const scope &scp) const {
	dst << "(";
	expr->py_translate(dst,scp);
	dst << ")";
}

void ParenExpr::print_mips(std::ostream &dst, context &program) const {
	expr->print_mips(dst,program);
}


std::string ParenExpr::get_ID() const{
	return "ParenExpr not supported for get_ID yet";
}



//-----------------------------------------------------------------------------
DivExpression::DivExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void DivExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "/";
	rhs->print_c(dst);
}

void DivExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "/";
	rhs->py_translate(dst,scp);
}

void DivExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();

	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tdiv $"  << dReg <<
		",$" << program.getnReg() << "\n";
	dst << "\tmflo $" << dReg << "\n";
	program.freeReg();
}

//-----------------------------------------------------------------------------
ModExpression::ModExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void ModExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "%";
	rhs->print_c(dst);
}

void ModExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "%";
	rhs->py_translate(dst,scp);
}

void ModExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	dst << "\n\t #ModExpression \n";
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tdiv $"  << dReg <<
		",$" << program.getnReg() << "\n";
	dst << "\tmfhi $" << dReg << "\n";
	program.freeReg();
}
//-----------------------------------------------------------------------------
LshiftExpression::LshiftExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void LshiftExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "<<";
	rhs->print_c(dst);
}

void LshiftExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "<<";
	rhs->py_translate(dst,scp);
}

void LshiftExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tsll $" << dReg << ",$" << dReg <<
		",$" << program.getnReg() << "\n";
	program.freeReg();
}
//-----------------------------------------------------------------------------
RshiftExpression::RshiftExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void RshiftExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << ">>";
	rhs->print_c(dst);
}

void RshiftExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << ">>";
	rhs->py_translate(dst,scp);
}

void RshiftExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tsra $" << dReg << ",$" << dReg <<
		",$" << program.getnReg() << "\n";
	program.freeReg();
}
//-----------------------------------------------------------------------------
LTEQExpression::LTEQExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void LTEQExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "<=";
	rhs->print_c(dst);
}

void LTEQExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "<=";
	rhs->py_translate(dst,scp);
}

void LTEQExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tslt $" << dReg << ",$" << program.getnReg() <<
		",$" << dReg << "\n";
	dst << "\txori $" << dReg << ",1\n";
	program.freeReg();
}
//-----------------------------------------------------------------------------
GTEQExpression::GTEQExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void GTEQExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << ">=";
	rhs->print_c(dst);
}

void GTEQExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << ">=";
	rhs->py_translate(dst,scp);
}

void GTEQExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tslt $" << dReg << ",$" << dReg <<
		",$" <<  program.getnReg() << "\n";
	dst << "\txori $" << dReg << ",1\n";
	program.freeReg();
}
//-----------------------------------------------------------------------------
NEQExpression::NEQExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void NEQExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "!=";
	rhs->print_c(dst);
}

void NEQExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "!=";
	rhs->py_translate(dst,scp);
}

void NEQExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	std::string eq = program.createLabel();
	std::string end = program.createLabel();

	//Run lhs where value is stored into lReg
	lhs->print_mips(dst,program);
	//Adjust availReg for rhs
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tbne $" << dReg << ",$" << program.getnReg() <<
		"," << eq << "\n";
	dst << "\tnop\n";
	dst << "\tli $" << dReg << ",0\n";
	dst << "\tb " << end << "\n";
	dst << eq << ":\n";
	dst << "\tli $" << dReg << ",1\n";
	dst << end <<":\n";


	program.freeReg();
}
//-----------------------------------------------------------------------------
BANDExpression::BANDExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void BANDExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "&";
	rhs->print_c(dst);
}

void BANDExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "&";
	rhs->py_translate(dst,scp);
}

void BANDExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	dst << "\n\t #XORExpression \n";
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tand $" << dReg << ",$" << dReg <<
		",$" << program.getnReg() << "\n";
	program.freeReg();
}
//-----------------------------------------------------------------------------
ExclusiveORExpression::ExclusiveORExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void ExclusiveORExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "^";
	rhs->print_c(dst);
}

void ExclusiveORExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "^";
	rhs->py_translate(dst,scp);
}

void ExclusiveORExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	dst << "\n\t #XORExpression \n";
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\txor $" << dReg << ",$" << dReg <<
		",$" << program.getnReg() << "\n";
	program.freeReg();
}
//-----------------------------------------------------------------------------
InclusiveORExpression::InclusiveORExpression(const Expression* _lhs, const Expression* _rhs) : 
	lhs(_lhs),rhs(_rhs){}

void InclusiveORExpression::print_c(std::ostream &dst) const {
	lhs->print_c(dst);
	dst << "|";
	rhs->print_c(dst);
}

void InclusiveORExpression::py_translate(std::ostream &dst, const scope &scp) const {
	lhs->py_translate(dst,scp);
	dst << "|";
	rhs->py_translate(dst,scp);
}

void InclusiveORExpression::print_mips(std::ostream &dst, context &program) const {
	int dReg = program.getnReg();
	dst << "\n\t #XORExpression \n";
	lhs->print_mips(dst,program);
	program.assignReg();
	rhs->print_mips(dst,program);
	dst << "\tor $" << dReg << ",$" << dReg <<
		",$" << program.getnReg() << "\n";
	program.freeReg();
}
//-----------------------------------------------------------------------------

PostIncrementExpr::PostIncrementExpr(const Identify* _postfixexpr)
 : postfixexpr(_postfixexpr){}


void PostIncrementExpr::print_c(std::ostream &dst) const {
	postfixexpr->print_c(dst);
	dst << "++\n";
}

void PostIncrementExpr::py_translate(std::ostream &dst, const scope &scp) const {
	postfixexpr->py_translate(dst,scp);
	dst << "++\n";
}


void PostIncrementExpr::print_mips(std::ostream &dst, context &program) const {

	postfixexpr->print_mips(dst,program);
	program.getnReg();
	// Identify* a = (Identify*)postfixexpr;
	std::string name = postfixexpr->get_ID();
	dst << "\taddiu $" << program.getnReg() << ",$" << program.getnReg() << ",1\n";
	dst << "\tsw $" << program.getnReg() << ","  << program.getFrameSize()- program.getVarOffset(name) << "($fp)\n";


}

std::string PostIncrementExpr::get_ID() const{
	return postfixexpr->get_ID();
}


//-----------------------------------------------------------------------------
