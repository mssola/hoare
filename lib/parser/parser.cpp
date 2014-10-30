/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "parser.h"
#include "node.h"
#include <iostream>

extern void yyparse(hoare::State *state);

using namespace hoare;

Parser::Parser()
{
	m_state.code = nullptr;
	m_state.line = 0;
	m_state.column = 0;
}

Parser::~Parser()
{
	m_state.problems.clear();
	if (m_state.code) {
		delete m_state.code;
	}
}

bool Parser::readFile(const std::string &path)
{
	int length;

	/* Open specified file */
	FILE *fd = fopen(path.c_str(), "r");
	if (!fd) {
		std::cout << "error: cannot open file: " << path << std::endl;
		fclose(fd);
		return false;
	}

	fseek(fd, 0, SEEK_END);
	length = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	if (!length) {
		fclose(fd);
		return false;
	}
	m_state.contents = (char *) malloc(sizeof(char) * length);

	if (!m_state.contents) {
		std::cout << "error: cannot open file: " << path << std::endl;
		fclose(fd);
		return false;
	}
	fread(m_state.contents, length, 1, fd);
	if (ferror(fd)) {
		std::cout << "error: cannot read file: " << path << std::endl;
		fclose(fd);
		return false;
	}
	m_state.length = length;
	m_state.lexer = m_state.contents;
	fclose(fd);
	return true;
}

void Parser::parse(std::string path)
{
	// Reset the previous state if we are coming from a previous execution.
	m_state.problems.clear();
	if (m_state.code) {
		delete m_state.code;
		m_state.code = nullptr;
	}

	// Parse the thing.
	if (readFile(path)) {
		yyparse(&m_state);
	}

	// Show all the problems that have occurred so far.
	for (auto &p : m_state.problems) {
		std::cout << path << ":" << p.line << ":" << p.column << ": "
			<< p.message << std::endl;
	}
}

