/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "driver.h"
#include "node.h"
#include <iostream>

extern void yyparse(hoare::Driver *);

using namespace hoare;

Driver::Driver()
{
	code = nullptr;
}

Driver::~Driver()
{
	if (code) {
		delete code;
	}
}

void Driver::parse(const std::string &path)
{
	// Reset the previous state if we are coming from a previous execution.
	problems = Problems(path);
	if (code) {
		delete code;
		code = nullptr;
	}

	// Parse the thing.
	scanner = Scanner(path);
	if (scanner.readFile()) {
		code = new NBlock();
		yyparse(this);
	}
}

void Driver::addProblem(const std::string &message)
{
	scanner.pushback();

	if (scanner.counter > 0) {
		problems << Problem(scanner.line, (scanner.column - scanner.counter),
			scanner.column, message);
	} else {
		problems << Problem(scanner.line, scanner.column, message);
	}
}

