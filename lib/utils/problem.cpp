/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "problem.h"
#include <llvm/Support/raw_ostream.h>
#include <fstream>
#include <cstdlib>
#include <unistd.h>

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
	: line(line), startColumn(startColumn), endColumn(endColumn), message(m)
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
	llvm::outs() << absolutePath(path) << ":";

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

std::string Problem::absolutePath(const std::string &path) const
{
	char buffer[256];
	auto dir = getcwd(buffer, 256);

	if (dir) {
		return std::string(dir) + "/" + path;
	}
	return path;
}

Problems::Problems()
	: Problems("")
{
}

Problems::Problems(const std::string &path)
	: path(path)
{
	problems.clear();
}

Problems::Problems(const Problems &problems)
	: path(problems.path), problems(problems.problems)
{
}

Problems::~Problems()
{
	problems.clear();
}

void Problems::addProblem(unsigned int line, unsigned int column,
	const std::string &message)
{
	Problem p(line, column, message);
	problems.push_back(p);
}

void Problems::addProblem(unsigned int line, unsigned int startColumn,
	unsigned int endColumn, const std::string &message)
{
	Problem p(line, startColumn, endColumn, message);
	problems.push_back(p);
}

void Problems::operator<<(const Problem &problem)
{
	problems.push_back(problem);
}

void Problems::print() const
{
	if (problems.empty()) {
		return;
	}

	for (const auto &p : problems) {
		p.print(path);
	}

	std::string str = std::to_string(problems.size()) + " error";
	if (problems.size() > 1) {
		str += "s";
	}
	str += " generated.\n";
	llvm::outs() << str;
}

