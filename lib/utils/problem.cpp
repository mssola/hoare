/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "problem.h"
#include <llvm/Support/raw_ostream.h>

using namespace hoare;

Problem::Problem(unsigned int l, unsigned int c, std::string m)
{
	line = l;
	column = c;
	message = m;
}

void Problem::print(const std::string &path)
{
	// First of all we print the location of the problem.
	llvm::outs().changeColor(llvm::raw_ostream::WHITE, true);
	llvm::outs() << path << ":";

	if (line != 0 && column != 0) {
		llvm::outs() << line << ":" << column << ":";
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

