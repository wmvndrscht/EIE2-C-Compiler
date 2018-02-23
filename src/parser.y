%code requires{

	#include "ast.hpp"
	#include <cassert>

	extern const Node *ast_root;

	int yylex(void);
	void yyerror(const char*);

}


%union{
	const Node *node;
	const DeclarationSpecifier *decspec;
	const CompoundStatement* cstatement;
	std::string *str;
	double *number;
}

/* token terminal */
%token T_TYPEDEF T_EXTERN T_STATIC T_AUTO T_REGISTER
%token T_VOID T_CHAR T_SHORT T_INT T_LONG T_FLOAT T_DOUBLE T_SIGNED T_UNSIGNED
%token T_CONST T_VOLATILE
%token T_SEMICOLON T_EQUAL 
%token T_LCBRACK T_RCBRACK T_LRBRACK T_RRBRACK
%token T_IDENTIFIER
%token T_RETURN
%token T_NUMBER


/* non-terminal */
%type<node> ROOT Translation_Unit External_Declaration 
%type<node> Function_Definition
%type<node> Declaration Declarator Init_Declarator_List Init_Declarator
%type<node>  Direct_Declarator
%type<cstatement> Compound_Statement
%type<node> Statement_List Statement Return_Statement
%type<node> Expression Primary_Expression
%type<decspec> Declaration_Specifiers
%type<str> Storage_Class_Specifier Type_Qualifier Type_Specifier T_IDENTIFIER
%type<number> T_NUMBER



%%

ROOT:	Translation_Unit 	{ ast_root = $1; }

Translation_Unit	:	External_Declaration 	{$$ = $1;}
									| Translation_Unit External_Declaration	{$$ = new TranslationUnit($1, $2);}

External_Declaration	:	Declaration 				{$$ = $1;}
											| Function_Definition	{$$ = $1;}

Function_Definition	:	Declaration_Specifiers Declarator Compound_Statement {$$ = new FunctionDefinition($1,$2,$3);}

Compound_Statement	: T_LCBRACK T_RCBRACK	{$$ = new CompoundStatement(NULL, NULL);}
										| T_LCBRACK Statement_List T_RCBRACK	{$$ = new CompoundStatement($2, NULL);}
										/*| T_LCBRACK Declaration_List T_RCBRACK *
										/* | T_LCBRACK Declaration_List Statement_List T_RCBRACK */


Declaration 	:	Declaration_Specifiers T_SEMICOLON {$$ = new LoneDeclaration($1);}
							|	Declaration_Specifiers Init_Declarator_List T_SEMICOLON	{$$ = new Declaration($1,$2); }

 /* Declaration_List	: Declaration
									| Declaration_List Declaration */

Statement_List	: Statement {$$ = $1;}
								/*| Statement_List	Statement {$$ = $1} */

Statement : Return_Statement	{$$ = $1;}

Return_Statement	:	T_RETURN T_SEMICOLON	{ $$ = new ReturnStatement(); }
									| T_RETURN Expression T_SEMICOLON {$$ = new ReturnExprStatement($2);}

Expression 	: Primary_Expression {$$ = $1;}

Primary_Expression	: T_IDENTIFIER	{$$ = new ExpressionVariable($1);}
										| T_NUMBER				{$$ = new Value($1);}

Init_Declarator_List 	: Init_Declarator {$$ = $1;}

Init_Declarator :	 Declarator {$$ = $1;}

Declarator 	:	Direct_Declarator		{$$ = $1;}

Direct_Declarator	:	T_IDENTIFIER	{$$ = new VariableDeclarator(*$1);}
									|	Direct_Declarator T_LRBRACK T_RRBRACK {$$ = new EmptyDeclarator($1);}

Declaration_Specifiers	:	Storage_Class_Specifier Declaration_Specifiers 	{$$ = new DeclarationSpecifier(*$1,$2); }
												|	Storage_Class_Specifier													{$$ = new DeclarationSpecifier(*$1,NULL); }
												| Type_Specifier Declaration_Specifiers						{$$ = new DeclarationSpecifier(*$1,$2); }				 
												| Type_Specifier																	{$$ = new DeclarationSpecifier(*$1,NULL); }	
												| Type_Qualifier Declaration_Specifiers						{$$ = new DeclarationSpecifier(*$1,$2); }
												| Type_Qualifier																	{$$ = new DeclarationSpecifier(*$1,NULL); }

Storage_Class_Specifier	:	T_TYPEDEF		{$$ = new std::string("typedef");}	
												| T_EXTERN		{$$ = new std::string("extern");}
												| T_STATIC		{$$ = new std::string("static");}
												| T_AUTO			{$$ = new std::string("auto");}
												| T_REGISTER	{$$ = new std::string("register");}

Type_Specifier	:	T_VOID			{$$ = new std::string("void");}
								| T_CHAR			{$$ = new std::string("char");}
								| T_SHORT			{$$ = new std::string("short");}
								| T_INT				{$$ = new std::string("int");}
								| T_LONG			{$$ = new std::string("long");}
								| T_FLOAT			{$$ = new std::string("float");}
								| T_DOUBLE		{$$ = new std::string("double");}
								| T_SIGNED 		{$$ = new std::string("signed");}
								| T_UNSIGNED	{$$ = new std::string("unsigned");}

Type_Qualifier	:	T_CONST			{$$ = new std::string("const");}
								| T_VOLATILE	{$$ = new std::string("volatile");}

 /* Assignment_Expression
						| Expression T_COMMA Assignment_Expression

Assignment_Expression	:	Conditional_Expression
											| Logical_OR_Expression


Conditional_Expression	: Logical_OR_Expression

Logical_OR_Expression	: Logical_AND_Expression

Logical_AND_Expression	: Inclusive_OR_Expressoin

Inclusive_OR_Expressoin	: Exclusive_OR_Expression

Exclusive_OR_Expression	:	Logical_AND_Expression

AND_Expression :	Equality_Expression

Relational_Expression	: Shift_Expression

Shift_Expression : Additive_Expression
	
*/

	/*
		Init_Declarator :	Declarator T_EQUAL Initializer
									| T_LPAREN Declarator T_RPAREN 
									| Direct_Declarator T_LBRACK T_RBRACK
									| Direct_Declarator T_LBRACK Constant_Expression T_RBRACK */


%%

const Node *ast_root; // Definition of variable (to match declaration earlier)

const Node *parseAST()
{
  ast_root=NULL;
  yyparse();
  return ast_root;
}
