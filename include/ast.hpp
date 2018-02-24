#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_node.hpp"
#include "ast/ast_statements.hpp"
#include "ast/ast_declarations.hpp"
#include "ast/ast_expressions.hpp"

extern const Node *pyparseAST(const char file[]);
extern const Node *cparseAST();


#endif