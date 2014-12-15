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

#include <llvm/Support/raw_ostream.h>
#include <codegen/hoare.h>

void usage()
{
	llvm::outs() << "usage: hoare [-S] <file>\n";
	exit(1);
}

void unknown(const std::string &arg)
{
	llvm::outs() << "hoare: error: unknown argument '" << arg << "'\n";
	usage();
}

int main(int argc, char *argv[])
{
	bool print = false;

	if (argc == 3) {
		if (strncmp(argv[1], "-S", 2) != 0) {
			unknown(argv[1]);
		}
		print = true;
	} else if (argc != 2) {
		usage();
	}

	hoare::Hoare h(argv[argc - 1]);
	return h.compile(print);
}

