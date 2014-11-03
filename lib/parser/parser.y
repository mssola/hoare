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
#include <parser/parser.h>

/* yy's functions */
static int yylex(void *lval, void *p);
static void yyerror(hoare::State *state, const char *s);

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
%pure-parser
%lex-param { hoare::State *state }
%parse-param { hoare::State *state }

%token<name> tNAME tSTRING
%token<numeric> tNUM
%token<token> tASSIGN tPRINTF tDOT2 tCOLON2 tOR tELLIPSIS tARROW tSQUARE
%token<token> tEQ tLEQ tLT tGEQ tGT tNE

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

%start program

%%

program: stmts
	{
		state->code = $1;
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
expression: integer_constant
	| structured_expression
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
subscripts_list: num
	| subscripts_list ',' num
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
		$$->line = state->line;
		$$->startColumn = (state->column - strlen($$->name.c_str()));
		$$->endColumn = state->column - 1;
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

#define IS_EOF() ((unsigned int) (state->lexer - state->contents) >= state->length)

/*
 * Some macros to make easier the UTF-8 support
 */
#define isUtf(c) ((c & 0xC0) != 0x80)
#define isSpecial(c) (utf8CharSize(c) > 1)

/*
 * This function is really simple. It steps over a char of
 * the string s, that is encoded in UTF-8. The result varies on the
 * number of bytes that encodes a single character following the UTF-8
 * rules. Therefore, this function will return 1 if the character
 * is in plain-ASCII, and greater than 1 otherwise.
 */
static int utf8CharSize(const char *s)
{
	int size = 0;
	int i = 0;

	do {
		i++;
		size++;
	} while (s[i] && !isUtf(s[i]));
	return size;
}

static int isUtf8Alpha(const char *str)
{
	return isSpecial(str) ? 1 : isalpha(*str);
}

static bool isValidIdentifier(const char *p)
{
	return (*p == '_' || isdigit(*p) || isUtf8Alpha(p));
}

#define nextc() parserNextCharacter(state)

static unsigned int parserNextCharacter(hoare::State *state)
{
	int c;

	if (IS_EOF()) {
		return EOF;
	}

	c = (unsigned char) *state->lexer++;
	state->prevc = state->column;
	if (c == '\n') {
		state->line++;
		state->column = -1;
	}
	state->column++;
	return c;
}

/* Move the lexer backwards. */
static void parserPushBack(hoare::State *state)
{
	state->column--;
	state->lexer--;
	if (*state->lexer == '\n') {
		state->line--;
		state->column = state->prevc;
	}
}
#define pushback() parserPushBack(state)

static char readEscape(hoare::State *state)
{
	int c;

	/*
	 * octals, \h, \u escape characters are not supported, too lazy :P
	 */

	switch (c = nextc()) {
	case '\\':
		return c;
	case '0':
		return '\0';
	case 'n':
		return '\n';
	case 't':
		return '\t';
	case 'r':
		return '\r';
	case 'f':
		return '\f';
	case 'v':
		return '\13';
	case 'a':
		return '\007';
	case 'e':
		return 033;
	case 'b':
		return '\010';
	case '\'':
		return '\'';
	case '"':
		return '"';
	}

	yyerror(state, "unknown escape character");
	return 0;
}

static int yylex(void *lval, void *p)
{
	// Some magic around the given parameters.
	YYSTYPE *yylval = static_cast<YYSTYPE *>(lval);
	hoare::State *state = static_cast<hoare::State *>(p);

	int c, aux;
	char *prev;
	std::string name = "";

retry:
	prev = state->lexer;
	c = nextc();

	switch (c) {
	case EOF:
	case '\0':
		return 0;
	case ' ': case '\t': case '\f': case '\r': case '\n': case '\13':
		goto retry;
	case '#':
		for (c = nextc(); c != EOF && c != '\n'; c = nextc());
		goto retry;
	case ':':
		if (*(state->lexer) == '=') {
			nextc();
			return tASSIGN;
		} else if (*(state->lexer) == ':') {
			nextc();
			return tCOLON2;
		}
		break;
	case '.':
		if (*(state->lexer) == '.') {
			nextc();
			if (*(state->lexer) == '.') {
				nextc();
				return tELLIPSIS;
			}
			return tDOT2;
		}
		break;
	case '|':
		if (*(state->lexer) == '|') {
			nextc();
			return tOR;
		}
		break;
	case '[':
		if (*(state->lexer) == ']') {
			nextc();
			return tSQUARE;
		}
		break;
	case '-':
		if (*(state->lexer) == '>') {
			nextc();
			return tARROW;
		}
		break;
	case '=':
		if (*(state->lexer) == '=') {
			nextc();
			return tEQ;
		}
		break;
	case '<':
		if (*(state->lexer) == '=') {
			nextc();
			return tLEQ;
		}
		return tLT;
	case '>':
		if (*(state->lexer) == '=') {
			nextc();
			return tGEQ;
		}
		return tGT;
	case '!':
		if (*(state->lexer) == '=') {
			nextc();
			return tNE;
		}
		break;
	case '"':
		c = nextc();
		while (c != '"') {
			if (c == EOF) {
				yyerror(state, "unterminated string");
				return 0;
			}
			if (c == '\\') {
				c = readEscape(state);
			}
			name += c;
			c = nextc();
		}
		yylval->name = new std::string(name);
		return tSTRING;
	default:
		// Check if this is a numeric value.
		if (isdigit(c)) {
			do {
				name += c;
				c = nextc();
			} while (isdigit(c));
			pushback();
			yylval->numeric = std::stoll(name);
			return tNUM;
		}

		// Check if this is a name.
		if (!isUtf8Alpha(prev) && *prev != '_') {
			break;
		}

		do {
			name += c;
			prev = state->lexer;
			c = nextc();
		} while (isValidIdentifier(prev));
		pushback();

		// Check if this is a keyword.
		if (name == "printf") {
			return tPRINTF;
		}

		yylval->name = new std::string(name);
		return tNAME;
	}
	return c;
}

static void yyerror(hoare::State *state, const char *s)
{
	pushback();
	hoare::Problem p(state->line, state->column, std::string(s));
	state->problems.push_back(p);
}

