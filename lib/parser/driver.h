/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_PARSER_H
#define HOARE_PARSER_H

#include <utils/problem.h>
#include <string>
#include "node.h"
#include "scanner.h"

namespace hoare {

class NCode;

class Driver
{
public:
	Driver();
	~Driver();

	void parse(const std::string &path);
	void addProblem(const std::string &message);

private:
	char readEscape(Driver *driver);

public:
	NCode *code;
	Scanner scanner;
	Problems problems;
};

}

#endif /* HOARE_PARSER_H */

