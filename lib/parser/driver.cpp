/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <parser/driver.h>

#include <iostream>

#include <ast/node.h>

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
