/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

%{

// TODO
#include <iostream>
#include <cstring>
#include <parser/node.h>
#include <parser/driver.h>
#include <parser/scanner.h>

#ifdef yyerror
#undef yyerror
#endif
#define yyerror(driver,msg) driver->addProblem(msg)


static int yylex(YYSTYPE *lval, void *p)
{
	hoare::Driver *driver = static_cast<hoare::Driver *>(p);
	return driver->scanner.lex(lval, driver);
}

%}

%union {
	hoare::Node *node;
	std::string *name;
	hoare::NString *string;
	hoare::NStatement *stmt;
	hoare::NCode *code;
	hoare::NExpression *expr;
	hoare::NName *ident;
	hoare::NNumeric *num;
	hoare::NExpressionList *exprlist;
	hoare::NStatementList *stmtlist;
	unsigned long long numeric;
	int token;
}

%define parse.error verbose
%define api.pure

%lex-param { hoare::Driver *driver }
%parse-param { hoare::Driver *driver }

// Tokens.

%token <name>
	tNAME		"name"
	tSTRING		"string"

%token <numeric>
	tNUM		"number"

%token <token>
	tASSIGN		"assignment"
	tPRINTF		"printf"
	tDOT2		"range"
	tCOLON2		"double colon"
	tOR			"separator"
	tELLIPSIS	"empty value"
	tARROW		"guard arrow"
	tSQUARE		"guard separator"
	tEQ			"equal comparison"
	tLEQ		"less or equal comparison"
	tLT			"less than comparison"
	tGEQ		"greater or equal comparison"
	tGT			"greater than comparison"
	tNE			"not equal comparison"
	tEND 0		"end of file"

// Rules.

%type<string> string
%type<ident> name
%type<num> num
%type<stmt> stmt assignment_command printf_command simple_command command
%type<stmt> declaration process structured_command parallel_command
%type<stmt> input_command output_command
%type<stmt> alternative_command repetitive_command
%type<code> stmts
%type<exprlist> args
%type<stmtlist> process_list

// Operator precedence.

%left tEQ tLNE tLEQ tLGEQ tLT tGT
%left '+' '-'
%left '*' '/' '%'
%left '('
%left ')'

// TODO: destructor

%start program

%%

program: stmts
	{
		driver->code = $1;
	}
;

stmts: stmt ';'
	{
		$$ = new hoare::NCode();
		if ($1) {
			$$->statements.push_back($<stmt>1);
		}
	}
	| stmts stmt ';'
	{
		if ($2) {
			$1->statements.push_back($<stmt>2);
		}
	}
;

stmt: declaration
	| command
;

declaration: name ':' name
	{
		$1->declaration = true;
		$<stmt>$ = new hoare::NDeclaration(*$1, *$3);
	}
;

command: simple_command
	| structured_command
;

simple_command: %empty
	{
		$$ = nullptr;
	}
	| printf_command
	| assignment_command
	| input_command
	| output_command
;

/* TODO */
assignment_command: target_variable tASSIGN expression
	{
	}
;

/* TODO */
expression: integer_expression
	| '(' integer_expression ')'
	| structured_expression
	| '(' structured_expression ')'
;

integer_expression: integer_constant
	| integer_constant '+' integer_constant
	| integer_constant '-' integer_constant
	| integer_constant '*' integer_constant
	| integer_constant '/' integer_constant
	| integer_constant '%' integer_constant
;

/* TODO */
structured_expression: num '(' opt_expression_list ')'
;

/* TODO */
constructor: %empty
	| name
;

/* TODO */
opt_expression_list: %empty
	| expression_list
;

/* TODO */
expression_list: expression
	| expression_list ',' expression
;

/* TODO */
target_variable: name
	| structured_target
;

/* TODO */
structured_target: constructor '(' opt_target_variable_list ')'
;

/* TODO */
opt_target_variable_list: %empty
	| target_variable_list
;

/* TODO */
target_variable_list: target_variable
	| target_variable_list ',' target_variable
;

/* TODO */
input_command: process_name '<' target_variable
	{
	}
;

/* TODO */
output_command: process_name '>' expression
	{
	}
;

/* TODO */
process_name: '-' name opt_subscripts_list
;

/* TODO */
opt_subscripts_list: %empty
	| '(' subscripts_list ')'
;

/* TODO */
subscripts_list: integer_expression
	| subscripts_list ',' integer_expression
;

printf_command: tPRINTF '(' args ')'
	{
		$<stmt>$ = new hoare::NFunctionCall(*$3);
	}
;

args: args ',' expr
	{
		$1->push_back($<expr>3);
	}
	| expr
	{
		$$ = new hoare::NExpressionList();
		$$->push_back($<expr>1);
	}
;

/* TODO */
structured_command: alternative_command
	| repetitive_command
	| parallel_command
;

/* TODO */
alternative_command: '[' guarded_command_list ']'
	{
	}
;

/* TODO */
guarded_command_list: guarded_command
	| guarded_command_list tSQUARE guarded_command
;

/* TODO */
guarded_command: guard tARROW stmts
	| '(' range_list ')' guard tARROW stmts
;

/* TODO */
range_list: range
	| range_list ',' range
;

/* TODO */
guard: guard_list
	| guard_list ';' input_command
	| input_command
;

/* TODO */
guard_list: guard_element
	| guard_list ';' guard_element
;

/* TODO */
guard_element: boolean_expression
	| declaration
;

/* TODO */
boolean_expression: integer_constant cmp integer_constant
;

/* TODO */
cmp: tEQ | tLEQ | tLT | tGEQ | tGT | tNE
;

/* TODO */
repetitive_command: '*' alternative_command
	{
	}
;

/* TODO */
parallel_command: '[' process_list ']'
	{
		auto /* p = */ np = new hoare::NParallel();
		np->statements = *$2;
		$$ = np;
	}
;

/* TODO */
process_list: process
	{
		$$ = new hoare::NStatementList();
		if ($1) {
			$$->push_back($<stmt>1);
		}
	}
	| process_list tOR process
	{
		if ($3) {
			$1->push_back($<stmt>3);
		}
	}
;

/* TODO */
process: process_label stmts
	{
		$$ = nullptr;
	}
	| tELLIPSIS
	{
		$$ = nullptr;
	}
;

/* TODO */
process_label: name tCOLON2
	{
	}
	| name '(' label_subscript_list ')' tCOLON2
	{
	}
;

/* TODO */
label_subscript_list: label_subscript
	| label_subscript_list ',' label_subscript
;

/* TODO */
label_subscript: integer_constant
	| range
;

/* TODO */
integer_constant: num
	| name
;

/* TODO */
range: name ':' num tDOT2 num
;

/* TODO */
expr: name
	| num
	| string
;

name: tNAME
	{
		$$ = new hoare::NName(*$1);
		$$->line = driver->scanner.line;
		$$->startColumn = (driver->scanner.column - strlen($$->name.c_str()));
		$$->endColumn = driver->scanner.column - 1;
		delete $1;
	}
;

num: tNUM
	{
		$$ = new hoare::NNumeric($1);
	}
;

string: tSTRING
	{
		$$ = new hoare::NString(*$<name>1);
		delete $1;
	}
;

%%

