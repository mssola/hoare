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

#include <llvm/Support/raw_ostream.h>
#include <codegen/hoare.h>

constexpr int major = 0;
constexpr int minor = 1;
constexpr int patch = 0;

void version()
{
	if (patch == 0) {
		printf("hoare version %d.%d\n", major, minor);
	} else {
		printf("hoare version %d.%d.%d\n", major, minor, patch);
	}
	exit(0);
}

void usage()
{
	llvm::outs() << "usage: hoare [options] <files>\n";
	llvm::outs() << "Options:\n";
	llvm::outs() << "  -h, --help    \tShow this message.\n";
	llvm::outs() << "  -S            \tEmit the LLVM representation assembler.\n";
	llvm::outs() << "  -v, --version \tShow the current version.\n";
	exit(0);
}

/**
 * Checks whether the given file not only exists, but it can also be read by
 * the current process.
 */
bool fileReadable(const char *path)
{
	FILE *file = fopen(path, "r");
	if (file != NULL) {
		fclose(file);
		return true;
	}

	std::string msg("hoare: error: " + std::string(path));
	perror(msg.c_str());
	return false;
}

int main(int argc, char *argv[])
{
	int index;
	bool print = false;

	for (index = 1; index < argc; index++) {
		std::string str(argv[index]);

		if (str == "-S") {
			print = true;
		} else if (str == "-h" || str == "--help") {
			usage();
		} else if (str == "-v" || str == "--version") {
			version();
		} else {
			if (str[0] == '-') {
				llvm::outs() <<
					"hoare: error: unrecognized command line option '" <<
					str << "'\n";
			} else {
				break;
			}
		}
	}

	bool compiled = false;
	for (; index < argc; index++) {
		if (!fileReadable(argv[index])) {
			continue;
		}
		if (!hoare::compile(argv[index], print)) {
			return 1;
		}
		compiled = true;
	}

	if (!compiled) {
		llvm::outs() << "hoare: error: no input files\n";
		exit(1);
	}
	return 0;
}

