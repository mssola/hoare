/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include <iostream>
#include <cstdlib>
#include <backend/hoare.h>

using namespace hoare;

void usage()
{
	std::cout << "usage: hoare [-S] <file>" << std::endl;
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

