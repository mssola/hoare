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

#include <codegen/hoare.h>

#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/JIT.h>

#include <codegen/context.h>
#include <codegen/printer.h>
#include <parser/driver.h>

using namespace hoare;

int Hoare::compile(const char *path, bool print)
{
	// First of all we have to parse the given file.
	std::string file(path);
	Driver driver;
	driver.parse(file);
	if (!driver.code) {
		driver.problems.print();
		return 0;
	}

	// LLVM requires this, otherwise it will crash.
	llvm::InitializeNativeTarget();

	// Generate the code.
	Context context(file, driver.problems);
	context.generateCode(driver.code);
	if (!context.problems.empty()) {
		return 0;
	}

	// The code could be compiled, now it's time to print/execute it.
	if (print) {
		Printer printer;
		printer.print(context);
	} else {
		context.run();
	}

	return 1;
}

