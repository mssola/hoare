/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include <backend/hoare.h>
#include <llvm/Support/raw_ostream.h>
#include <cstdlib>

using namespace hoare;

void usage()
{
	llvm::outs() << "usage: hoare [-S] <file>\n";
}

int main(int argc, char *argv[])
{
	bool print = false;

	if (argc == 3) {
		print = true;
	} else if (argc != 2) {
		usage();
		exit(1);
	}

	Hoare h(argv[argc - 1]);
	return h.compile(print);
}

