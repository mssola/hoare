/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_PROBLEM_H
#define HOARE_PROBLEM_H

#include <string>
#include <vector>

namespace hoare {

class Problem
{
public:
	Problem();
	explicit Problem(unsigned int l, unsigned int c, std::string m);
	explicit Problem(unsigned int line, unsigned int startColumn,
		unsigned int endColumn, std::string m);

	void print(const std::string &path) const;

private:
	void printReport(const std::string &path) const;
	void printLine(const std::string &path) const;
	std::string absolutePath(const std::string &path) const;

public:
	unsigned int line, startColumn, endColumn;
	std::string message;
};

class Problems
{
public:
	Problems();
	explicit Problems(const std::string &path);
	explicit Problems(const Problems &problems);
	virtual ~Problems();

	void addProblem(unsigned int line, unsigned int column,
		const std::string &message);
	void addProblem(unsigned int line, unsigned int startColumn,
		unsigned int endColumn, const std::string &message);
	void operator<<(const Problem &problem);

	inline const bool empty() const
	{
		return problems.empty();
	}

	void print() const;

private:
	std::string path;
	std::vector<Problem> problems;
};

}

#endif /* HOARE_PROBLEM_H */

