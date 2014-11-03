/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_LEXER_H
#define HOARE_LEXER_H


#define nextc() parserNextCharacter(state)
#define pushback() parserPushBack(state)

/* yy's functions */
int yylex(void *lval, void *p);
void yyerror(hoare::State *state, const char *s);


#endif /* HOARE_LEXER_H */

