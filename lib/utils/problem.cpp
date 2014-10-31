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

Problem::Problem(unsigned int l, unsigned int c, std::string m)
	: Problem(l, c, c, m)
{
}

Problem::Problem(unsigned int line, unsigned int startColumn,
	unsigned int endColumn, std::string m)
	: line(line), startColumn(startColumn), endColumn(endColumn), message(m)
{
}

void Problem::print(const std::string &path)
{
	printReport(path);
	printLine(path);
	llvm::outs().changeColor(llvm::raw_ostream::WHITE);
}

void Problem::printReport(const std::string &path)
{
	// First of all we print the location of the problem.
	llvm::outs().changeColor(llvm::raw_ostream::WHITE, true);
	llvm::outs() << absolutePath(path) << ":";

	if (line != 0 && startColumn != 0 && endColumn != 0) {
		llvm::outs() << line << ":" << startColumn << ":";
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

void Problem::printLine(const std::string &path)
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

