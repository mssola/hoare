/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_PARSER_H
#define HOARE_PARSER_H

#include <string>
#include "node.h"

namespace hoare {

class NCode;

// TODO
typedef struct {
	NCode *code;
	std::vector<Problem> problems;

	unsigned int line, column, prevc;
	char *contents;
	unsigned int length;
	char *lexer;
} State;

class Parser
{
public:
	Parser();
	~Parser();

	void parse(std::string path);

	inline NCode * code() const
	{
		return m_state.code;
	}

private:
	bool readFile(const std::string &path);

private:
	State m_state;
};

}

#endif /* HOARE_PARSER_H */

