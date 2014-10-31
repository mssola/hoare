/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_PROBLEM_H
#define HOARE_PROBLEM_H

#include <string>

namespace hoare {

class Problem
{
public:
	explicit Problem(unsigned int l, unsigned int c, std::string m);
	explicit Problem(unsigned int line, unsigned int startColumn,
		unsigned int endColumn, std::string m);

	void print(const std::string &path);

private:
	void printReport(const std::string &path);
	void printLine(const std::string &path);
	std::string absolutePath(const std::string &path) const;

public:
	unsigned int line, startColumn, endColumn;
	std::string message;
};

}

#endif /* HOARE_PROBLEM_H */

