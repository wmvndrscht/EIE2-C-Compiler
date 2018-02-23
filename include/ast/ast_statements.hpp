#ifndef ast_statements_hpp
#define ast_statements_hpp


#include <string>
#include <iostream>


class ReturnStatement : public Node {
public:
	virtual void print(std::ostream &dst) const override{
		dst<< "return;";
		//std::cerr << "[EmptyCompoundStatement]" << std::endl;
	}
	virtual void py_translate(std::ostream &dst) const override{
		dst << "\n **Return; not py yet...** \n";
	}
};

class ReturnExprStatement : public Node {
private:
	const NodePtr retexprstat;
public:
	ReturnExprStatement(const NodePtr _retexprstat) : retexprstat(_retexprstat){}
	virtual void print(std::ostream &dst) const override{
		dst << "return ";
		retexprstat->print(dst);
		dst << ";";
	}
	virtual void py_translate(std::ostream &dst) const override{
		dst << "return ";
		retexprstat->py_translate(dst);
	}
};

class CompoundStatement : public Node{
private:
	const NodePtr statlist;
	const NodePtr declist;
public:
	CompoundStatement(const NodePtr _statlist, const NodePtr _declist) : 
		statlist(_statlist), declist(_declist){}
	virtual void print(std::ostream &dst) const override{
		dst<< "{";
		if(statlist != NULL){
			dst << "\n";
			statlist->print(dst);
		}
		if(declist != NULL){
			dst << "\n";
			declist->print(dst);
		}
		dst<< "\n}";
		//std::cerr << "[EmptyCompoundStatement]" << std::endl;
	}
	virtual void py_translate(std::ostream &dst) const override{
		dst << ":\n";
		dst << "  ";
		if(statlist != NULL){
			statlist->py_translate(dst);
		}
		if(declist != NULL){
			declist->py_translate(dst);
		}
		dst << "\n";
	}
};



#endif