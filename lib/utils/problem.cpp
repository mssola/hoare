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

#include <utils/problem.h>

#include <fstream>

#include <llvm/Support/raw_ostream.h>

using namespace hoare;

Problem::Problem()
	: Problem(0, 0, 0, "")
{
}

Problem::Problem(unsigned int l, unsigned int c, std::string m)
	: Problem(l, c, c, m)
{
}

Problem::Problem(unsigned int line, unsigned int startColumn,
	unsigned int endColumn, std::string m)
	: line(line)
	, startColumn(startColumn)
	, endColumn(endColumn)
	, message(m)
{
}

void Problem::print(const std::string &path) const
{
	printReport(path);
	printLine(path);
	llvm::outs().changeColor(llvm::raw_ostream::WHITE);
}

void Problem::printReport(const std::string &path) const
{
	// First of all we print the location of the problem.
	llvm::outs().changeColor(llvm::raw_ostream::WHITE, true);
	llvm::outs() << path << ":";

	if (line != 0 && startColumn != 0 && endColumn != 0) {
		llvm::outs() << line + 1 << ":" << startColumn + 1 << ":";
	}

	// We print that this is an error.
	llvm::outs().changeColor(llvm::raw_ostream::RED, true);
	llvm::outs() << " error: ";

	// And we print the error message.
	llvm::outs().changeColor(llvm::raw_ostream::WHITE, true);
	llvm::outs() << message << ".";

	// And print the end of line with no more bolding.
	llvm::outs().changeColor(llvm::raw_ostream::WHITE);
	llvm::outs() << "\n";
}

void Problem::printLine(const std::string &path) const
{
	std::string current;
	std::ifstream in(path);

	for (int i = 0; i < line + 1; i++) {
		getline(in, current);
	}
	if (current == "") {
		return;
	}

	// Point the start of the word.
	std::string high;
	for (int i = 0; i < startColumn; i++) {
		if (current[i] == '\t') {
			high += '\t';
		} else {
			high += " ";
		}
	}
	high += "^";

	// Highlight the word.
	for (int i = startColumn; i < endColumn; i++) {
		high += "~";
	}

	llvm::outs() << current << "\n";
	llvm::outs().changeColor(llvm::raw_ostream::GREEN, true);
	llvm::outs() << high << "\n";
}

Problems::Problems()
	: Problems("")
{
}

Problems::Problems(const std::string &path)
	: m_path(path)
{
	m_problems.clear();
}

Problems::Problems(const Problems &problems)
	: m_path(problems.m_path)
	, m_problems(problems.m_problems)
{
}

Problems::~Problems()
{
	m_problems.clear();
}

void Problems::addProblem(unsigned int line, unsigned int column,
	const std::string &message)
{
	Problem p(line, column, message);
	m_problems.push_back(p);
}

void Problems::addProblem(unsigned int line, unsigned int startColumn,
	unsigned int endColumn, const std::string &message)
{
	Problem p(line, startColumn, endColumn, message);
	m_problems.push_back(p);
}

void Problems::operator<<(const Problem &problem)
{
	m_problems.push_back(problem);
}

void Problems::print() const
{
	if (m_problems.empty()) {
		return;
	}

	for (const auto &p : m_problems) {
		p.print(m_path);
	}

	std::string str = std::to_string(m_problems.size()) + " error";
	if (m_problems.size() > 1) {
		str += "s";
	}
	str += " generated.\n";
	llvm::outs() << str;
}
