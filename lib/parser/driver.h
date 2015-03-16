/*
 * Copyright (C) 2014-2015 Miquel Sabaté Solà <mikisabate@gmail.com>
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

#ifndef HOARE_PARSER_H
#define HOARE_PARSER_H

#include <memory>
#include <string>

#include <parser/scanner.h>
#include <utils/problem.h>

namespace hoare {

struct NBlock;

class Driver {
public:
	Driver();
	virtual ~Driver();

	void parse(const std::string &path);
	void addProblem(const std::string &message, bool pushback = true);

private:
	char readEscape(Driver *driver);

public:
	NBlock *code;
	std::unique_ptr<Scanner> scanner;
	Problems problems;
};

}

#endif // HOARE_PARSER_H
