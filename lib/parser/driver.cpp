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
	problems.clear();
}

Driver::~Driver()
{
	problems.clear();
	if (code) {
		delete code;
	}
}

void Driver::parse(const std::string &path)
{
	// Reset the previous state if we are coming from a previous execution.
	problems.clear();
	if (code) {
		delete code;
		code = nullptr;
	}

	// Parse the thing.
	scanner = Scanner(path);
	if (scanner.readFile()) {
		yyparse(this);
	}

	// Show all the problems that have occurred so far.
	for (auto &p : problems) {
		p.print(path);
	}
}

void Driver::addProblem(const std::string &message)
{
	Problem p;
	scanner.pushback();

	if (scanner.counter > 0) {
		p = Problem(scanner.line, (scanner.column - scanner.counter),
			scanner.column, message);
	} else {
		p = Problem(scanner.line, scanner.column, message);
	}
	problems.push_back(p);
}

