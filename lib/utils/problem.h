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

#ifndef HOARE_PROBLEM_H
#define HOARE_PROBLEM_H

#include <string>
#include <vector>

namespace hoare {

class Problem
{
public:
	Problem();
	Problem(unsigned int l, unsigned int c, std::string m);
	Problem(unsigned int line, unsigned int startColumn,
		unsigned int endColumn, std::string m);

	void print(const std::string &path) const;

private:
	void printReport(const std::string &path) const;
	void printLine(const std::string &path) const;

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

	inline bool empty() const
	{
		return m_problems.empty();
	}

	void print() const;

private:
	std::string m_path;
	std::vector<Problem> m_problems;
};

}

#endif // HOARE_PROBLEM_H
